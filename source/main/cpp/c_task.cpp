#include "rdno_core/c_target.h"
#include "rdno_core/c_malloc.h"
#include "rdno_core/c_memory.h"
#include "rdno_core/c_serial.h"
#include "rdno_core/c_system.h"
#include "rdno_core/c_task.h"
#include "rdno_core/c_timer.h"
#include "rdno_core/c_allocator.h"

namespace ncore
{
    namespace ntask
    {
        struct program_info_t
        {
            const byte* begin;
            const byte* end;
        };

        struct executor_t
        {
            program_info_t* m_programs;
            s32             m_max_programs;
            s32             m_num_programs;
            byte*           m_program_memory;
            const byte*     m_program_memory_end;
            byte*           m_scope_open;
            s32             m_scope;
            program_t       m_programming_program;
            byte*           m_programming_cursor;
            program_t       m_running_program;

            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        executor_t* init(s32 max_programs, s32 program_mem_size)
        {
            executor_t* exec = nsystem::construct<executor_t>();

            exec->m_programs     = nsystem::construct_array<program_info_t>(max_programs);
            exec->m_max_programs = max_programs;
            exec->m_num_programs = 0;

            exec->m_program_memory     = nsystem::malloc(program_mem_size);
            exec->m_program_memory_end = exec->m_program_memory + program_mem_size;
            exec->m_programming_cursor = exec->m_program_memory;
            exec->m_scope_open         = nullptr;
            exec->m_scope              = 0;

            exec->m_programming_program = nullptr;
            exec->m_running_program     = nullptr;

            return exec;
        }

        void boot(executor_t* exec, program_t program) { exec->m_running_program = program; }

        program_t program(executor_t* exec)
        {
            if (exec->m_num_programs >= exec->m_max_programs)
                return nullptr;

            exec->m_programs[exec->m_num_programs].begin = exec->m_programming_cursor;
            exec->m_programs[exec->m_num_programs].end   = nullptr;
            return &exec->m_programs[exec->m_num_programs++];
        }

        struct jump_t
        {
            program_t m_program;
        };

        struct run_t
        {
            program_t m_program;
        };

        struct run_periodic_t
        {
            u64        m_lastTime;
            u32        m_period_ms;
            function_t m_function;
        };

        struct once_t
        {
            u32        m_executed;
            function_t m_function;
        };

        struct opcode_if_func_t
        {
            function_t m_function;
            byte*      m_end;
        };

        struct opcode_if_timeout_t
        {
            u32   m_timeout;
            u64   m_startTime;
            byte* m_end;
        };

        typedef u8     opcode_t;
        const opcode_t OPCODE_JUMP         = 5;
        const opcode_t OPCODE_RUN          = 10;
        const opcode_t OPCODE_RUN_PERIODIC = 12;
        const opcode_t OPCODE_ONCE         = 15;
        const opcode_t OPCODE_IF_FUNC      = 20;
        const opcode_t OPCODE_IF_TIMEOUT   = 25;
        const opcode_t OPCODE_RET          = 30;

        // opcode
        // argument size
        //   ... alignment ...
        //   argument data[size]

        static byte* write_opcode_and_arg(byte* program_mem, opcode_t opcode, byte const* arg_data, s32 arg_size)
        {
            if (arg_data == nullptr || arg_size == 0)
            {
                *program_mem++ = opcode;
                *program_mem++ = 0;
                return program_mem;
            }

            byte* aligned_program_mem = nmem::ptr_align(program_mem + 2, sizeof(void*));
            program_mem[0]            = opcode;
            program_mem[1]            = (u8)(arg_size);
            for (s32 i = 0; i < arg_size; i++)
                *aligned_program_mem++ = *arg_data++;
            return aligned_program_mem;
        }

        static const byte* read_opcode_and_arg_size(const byte* pc, opcode_t& opcode, s32& arg_size)
        {
            opcode   = (opcode_t)(pc[0]);
            arg_size = (s32)pc[1];
            pc       = pc + 2;
            return arg_size > 0 ? nmem::ptr_align(pc, sizeof(void*)) : pc;
        }

        static byte* write(byte* program_mem, jump_t const& j) { return write_opcode_and_arg(program_mem, OPCODE_JUMP, (const byte*)&j, sizeof(j)); }
        static byte* write(byte* program_mem, run_t const& r) { return write_opcode_and_arg(program_mem, OPCODE_RUN, (const byte*)&r, sizeof(r)); }
        static byte* write(byte* program_mem, run_periodic_t const& pp) { return write_opcode_and_arg(program_mem, OPCODE_RUN_PERIODIC, (const byte*)&pp, sizeof(pp)); }
        static byte* write(byte* program_mem, once_t const& op) { return write_opcode_and_arg(program_mem, OPCODE_ONCE, (const byte*)&op, sizeof(op)); }
        static byte* write(byte* program_mem, opcode_if_func_t const& op) { return write_opcode_and_arg(program_mem, OPCODE_IF_FUNC, (const byte*)&op, sizeof(op)); }
        static byte* write(byte* program_mem, opcode_if_timeout_t const& op) { return write_opcode_and_arg(program_mem, OPCODE_IF_TIMEOUT, (const byte*)&op, sizeof(op)); }

        static byte* set_if_end(byte* if_start, byte* if_end)
        {
            opcode_t    opcode;
            s32         arg_size;
            const byte* if_arg_start = read_opcode_and_arg_size(if_start, opcode, arg_size);

            byte* outer_if = nullptr;
            if (opcode == OPCODE_IF_FUNC)
            {
                opcode_if_func_t* op = (opcode_if_func_t*)if_arg_start;
                outer_if             = op->m_end;
                op->m_end            = if_end;
            }
            else if (opcode == OPCODE_IF_TIMEOUT)
            {
                opcode_if_timeout_t* op = (opcode_if_timeout_t*)if_arg_start;
                outer_if                = op->m_end;
                op->m_end               = if_end;
            }
            return outer_if;
        }

        void xbegin(executor_t* exec, program_t program)
        {
            exec->m_programming_program        = program;
            exec->m_programming_program->begin = exec->m_programming_cursor;
            exec->m_scope                      = 1;
            exec->m_scope_open                 = nullptr;
        }

        void xjump(executor_t* exec, program_t program)
        {
            jump_t j                   = {program};
            exec->m_programming_cursor = write(exec->m_programming_cursor, j);
        }

        void xrun(executor_t* exec, program_t program)
        {
            run_t r                    = {program};
            exec->m_programming_cursor = write(exec->m_programming_cursor, r);
        }

        void xrun_periodic(executor_t* exec, function_t fn, u32 period_ms)
        {
            run_periodic_t pp          = {0, period_ms, fn};
            exec->m_programming_cursor = write(exec->m_programming_cursor, pp);
        }

        void xonce(executor_t* exec, function_t fn)
        {
            once_t op                  = {0, fn};
            exec->m_programming_cursor = write(exec->m_programming_cursor, op);
        }

        void xif(executor_t* exec, function_t fn)
        {
            opcode_if_func_t op        = {fn, exec->m_scope_open};
            exec->m_scope_open         = exec->m_programming_cursor;
            exec->m_programming_cursor = write(exec->m_programming_cursor, op);
            exec->m_scope++;
        }

        void xif(executor_t* exec, timeout_t t)
        {
            opcode_if_timeout_t op     = {t.m_timeout_ms, 0, exec->m_scope_open};
            exec->m_scope_open         = exec->m_programming_cursor;
            exec->m_programming_cursor = write(exec->m_programming_cursor, op);
            exec->m_scope++;
        }

        void xreturn(executor_t* exec) { exec->m_programming_cursor = write_opcode_and_arg(exec->m_programming_cursor, OPCODE_RET, nullptr, 0); }

        void xend(executor_t* exec)
        {
            // we only need to process scope management, there is no opcode
            if (exec->m_scope > 1)
            {
                exec->m_scope--;
                exec->m_scope_open = set_if_end(exec->m_scope_open, exec->m_programming_cursor);
            }
            else if (exec->m_scope == 1)
            {
                exec->m_scope                    = 0;
                exec->m_programming_program->end = exec->m_programming_cursor;
                exec->m_programming_program      = nullptr;  // Programming has ended
            }
        }

        static void reset_program(program_t program)
        {
            const byte* pc = (const byte*)program->begin;
            while (pc < program->end)
            {
                opcode_t opcode;
                s32      size;
                pc = read_opcode_and_arg_size(pc, opcode, size);

                switch (opcode)
                {
                    case OPCODE_ONCE:
                    {
                        once_t* op     = (once_t*)pc;
                        op->m_executed = 0;
                    }
                    break;
                    case OPCODE_IF_TIMEOUT:
                    {
                        opcode_if_timeout_t* op = (opcode_if_timeout_t*)pc;
                        op->m_startTime         = 0;
                    }
                    break;
                    case OPCODE_RUN_PERIODIC:
                    {
                        run_periodic_t* pp = (run_periodic_t*)pc;
                        pp->m_lastTime     = 0;
                    }
                    break;
                }  // switch

                pc += size;
            }  // while
        }

        static s32 program_pc(program_info_t* program, const byte* pc)
        {
            return (s32)(pc - program->begin);
        }

//#define PROGRAM_PRINT(a) nserial::println(a)
//#define PROGRAM_PRINTF(a, b) nserial::printf(a, b)

#define PROGRAM_PRINT(a) 
#define PROGRAM_PRINTF(a, b) 

        static void execute_program(program_info_t* program, state_t* state, program_info_t** new_program)
        {
            PROGRAM_PRINT("------------ EXECUTE PROGRAM ------------");
            const byte* pc = program->begin;
            while (pc < program->end)
            {
                opcode_t opcode;
                s32      size;
                pc = read_opcode_and_arg_size(pc, opcode, size);

                switch (opcode)
                {
                    case OPCODE_JUMP:
                    {
                        PROGRAM_PRINTF("JUMP %d\n", va_t(program_pc(program, pc)));
                        jump_t* j    = (jump_t*)pc;
                        *new_program = j->m_program;
                        return;
                    }
                    break;

                    case OPCODE_RUN:
                    {
                        PROGRAM_PRINTF("RUN %d\n", va_t(program_pc(program, pc)));
                        run_t* r = (run_t*)pc;
                        pc       = pc + size;
                        execute_program(r->m_program, state, new_program);
                    }
                    break;

                    case OPCODE_RUN_PERIODIC:
                    {
                        run_periodic_t* pp    = (run_periodic_t*)pc;
                        pc                    = pc + size;
                        const u64 currentTime = ntimer::millis();
                        if (pp->m_lastTime == 0 || (currentTime - pp->m_lastTime) >= pp->m_period_ms)
                        {
                            PROGRAM_PRINTF("RUN PERIODIC %d\n", va_t(program_pc(program, pc)));
                            if (pp->m_lastTime == 0)
                            {
                                pp->m_lastTime = currentTime;  // first run, we just set the last time
                            }
                            else
                            {
                                pp->m_lastTime += pp->m_period_ms;  // next run, we set the last time to the next period
                            }

                            pp->m_function(state);
                            // TODO do we need to handle the result?
                        }
                    }
                    break;

                    case OPCODE_ONCE:
                    {
                        once_t* op = (once_t*)pc;
                        pc         = pc + size;
                        if (op->m_executed == 0)
                        {
                            PROGRAM_PRINTF("ONCE %d\n", va_t(program_pc(program, pc)));
                            const result_t result = op->m_function(state);
                            op->m_executed        = 1;
                            // TODO do we need to handle the result?
                        }
                    }
                    break;

                    case OPCODE_IF_FUNC:
                    {
                        opcode_if_func_t* op  = (opcode_if_func_t*)pc;
                        pc                    = pc + size;
                        const result_t result = op->m_function(state);
                        if (result == RESULT_OK || result == RESULT_ERROR)
                        {
                            PROGRAM_PRINTF("IF FUNC - SKIP %d\n", va_t(program_pc(program, pc)));

                            // condition not met, so we skip to the end of this if
                            pc = op->m_end;
                        }
                        else if (result == RESULT_DONE)
                        {
                            PROGRAM_PRINTF("IF FUNC - CONTINUE %d\n", va_t(program_pc(program, pc)));
                            // condition met, we continue execution
                        }
                    }
                    break;

                    case OPCODE_IF_TIMEOUT:
                    {
                        opcode_if_timeout_t* op = (opcode_if_timeout_t*)pc;
                        pc                      = pc + size;

                        const u64 currentTime = ntimer::millis();
                        if (op->m_startTime == 0)
                        {
                            op->m_startTime = currentTime;
                            // timeout initialized, so we skip to the end of this if
                            PROGRAM_PRINTF("IF TIMEOUT - SKIP %d\n", va_t(program_pc(program, pc)));
                            pc = op->m_end;
                        }
                        else if (currentTime - op->m_startTime >= op->m_timeout)
                        {
                            // timeout reached, we continue execution of the scope
                            PROGRAM_PRINTF("IF TIMEOUT - CONTINUE %d\n", va_t(program_pc(program, pc)));
                        }
                        else
                        {
                            // timeout not yet reached, so we skip to the end of this if
                            PROGRAM_PRINTF("IF TIMEOUT - SKIP %d\n", va_t(program_pc(program, pc)));
                            pc = op->m_end;
                        }
                    }
                    break;

                    case OPCODE_RET:
                    {
                        PROGRAM_PRINTF("RETURN %d\n", va_t(program_pc(program, pc)));
                        return;
                    }

                }  // switch
            }  // while

            // we reached the end of the program, so we exit
        }

        void tick(executor_t* exec, state_t* state)
        {
            if (exec->m_running_program == nullptr)
            {
                nserial::println("No running program");
                return;
            }

            program_info_t* nextProgram = nullptr;
            execute_program(exec->m_running_program, state, &nextProgram);

            if (nextProgram != nullptr)
            {
                reset_program(nextProgram);
                exec->m_running_program = nextProgram;
            }
        }

    }  // namespace ntask
}  // namespace ncore