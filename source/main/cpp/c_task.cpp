#include "rdno_core/c_target.h"
#include "rdno_core/c_task.h"
#include "rdno_core/c_timer.h"

namespace ncore
{
    namespace ntask
    {
        byte gProgramMemory[1024];
        s32  gProgramMemoryUsed = 0;

        struct program_info_t
        {
            const byte* begin;
            const byte* end;
        };

        program_info_t gPrograms[8];
        s32            gMaxPrograms = sizeof(gProgramMemory) / sizeof(program_info_t);
        s32            gNumPrograms = 0;

        program_t scheduler_t::program()
        {
            gPrograms[gNumPrograms].begin = nullptr;
            gPrograms[gNumPrograms].end   = nullptr;
            return &gPrograms[gNumPrograms++];
        }

        struct jump_t
        {
            program_t m_program;
        };

        struct run_t
        {
            program_t m_program;
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

        typedef u16    opcode_t;
        const opcode_t OPCODE_JUMP       = 5 + ((sizeof(jump_t) & 0xFF) << 8);
        const opcode_t OPCODE_RUN        = 10 + ((sizeof(run_t) & 0xFF) << 8);
        const opcode_t OPCODE_ONCE       = 15 + ((sizeof(once_t) & 0xFF) << 8);
        const opcode_t OPCODE_IF_FUNC    = 20 + ((sizeof(opcode_if_func_t) & 0xFF) << 8);
        const opcode_t OPCODE_IF_TIMEOUT = 25 + ((sizeof(opcode_if_timeout_t) & 0xFF) << 8);
        const opcode_t OPCODE_RET        = 30 + (0 << 8);

        static byte* write(byte* program_mem, byte const* data, s32 size)
        {
            for (s32 i = 0; i < size; i++)
                *program_mem++ = *data++;
            return program_mem;
        }
        inline static byte* write(byte* program_mem, opcode_t opcode)
        {
            *program_mem++ = opcode;
            return program_mem;
        }
        static byte* write(byte* program_mem, jump_t const& j)
        {
            program_mem = write(program_mem, OPCODE_JUMP);
            return write(program_mem, (const byte*)&j, sizeof(j));
        }
        static byte* write(byte* program_mem, run_t const& r)
        {
            program_mem = write(program_mem, OPCODE_RUN);
            return write(program_mem, (const byte*)&r, sizeof(r));
        }
        static byte* write(byte* program_mem, once_t const& op)
        {
            program_mem = write(program_mem, OPCODE_ONCE);
            return write(program_mem, (const byte*)&op, sizeof(op));
        }
        static byte* write(byte* program_mem, opcode_if_func_t const& op)
        {
            program_mem = write(program_mem, OPCODE_IF_FUNC);
            return write(program_mem, (const byte*)&op, sizeof(op));
        }
        static byte* write(byte* program_mem, opcode_if_timeout_t const& op)
        {
            program_mem = write(program_mem, OPCODE_IF_TIMEOUT);
            return write(program_mem, (const byte*)&op, sizeof(op));
        }

        static byte* set_if_end(byte* if_start, byte* if_end)
        {
            opcode_t opcode   = *(opcode_t*)if_start;
            byte*    outer_if = nullptr;
            if (opcode == OPCODE_IF_FUNC)
            {
                opcode_if_func_t* op = (opcode_if_func_t*)(if_start + sizeof(opcode_t));
                outer_if             = op->m_end;
                op->m_end            = if_end;
            }
            else if (opcode == OPCODE_IF_TIMEOUT)
            {
                opcode_if_timeout_t* op = (opcode_if_timeout_t*)(if_start + sizeof(opcode_t));
                outer_if                = op->m_end;
                op->m_end               = if_end;
            }
            return outer_if;
        }

        void scheduler_t::xbegin(program_t program)
        {
            m_cursor                = &gProgramMemory[gProgramMemoryUsed];
            m_scope                 = 0;
            m_currentProgram->begin = m_cursor;
        }

        void scheduler_t::xjump(program_t program)
        {
            jump_t j      = {program};
            m_program_mem = write(m_program_mem, j);
        }

        void scheduler_t::xrun(program_t program)
        {
            run_t r       = {program};
            m_program_mem = write(m_program_mem, r);
        }

        void scheduler_t::xonce(function_t fn)
        {
            once_t op     = {0, fn};
            m_program_mem = write(m_program_mem, op);
        }

        void scheduler_t::xif(function_t fn)
        {
            opcode_if_func_t op = {fn, m_scope_open};
            m_scope_open        = m_program_mem;
            m_program_mem       = write(m_program_mem, op);
            m_scope++;
        }

        void scheduler_t::xif(timeout_t t)
        {
            opcode_if_timeout_t op = {t.m_timeout_ms, 0, m_scope_open};
            m_scope_open           = m_program_mem;
            m_program_mem          = write(m_program_mem, op);
            m_scope++;
        }

        void scheduler_t::xreturn() { m_program_mem = write(m_program_mem, OPCODE_RET); }

        void scheduler_t::xend()
        {
            // we only need to process scope management, there is no opcode
            if (m_scope > 1)
            {
                m_scope--;
                m_scope_open = set_if_end(m_scope_open, m_program_mem);
            }
            else if (m_scope == 1)
            {
                m_scope               = 0;
                m_currentProgram->end = m_program_mem;
                gProgramMemoryUsed    = s32(m_program_mem - gProgramMemory);
            }
        }

        void scheduler_t::initialize()
        {
            m_program_mem    = gProgramMemory;
            m_currentProgram = nullptr;
        }

        static void reset_program(program_t program)
        {
            byte* pc = (byte*)program->begin;
            while (pc < program->end)
            {
                const opcode_t opcode = *(opcode_t*)pc;
                pc += sizeof(opcode_t);

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
                }  // switch

                pc += (opcode >> 8) & 0xFF;
            }  // while
        }

        static void execute_program(program_info_t* program, state_t* state, program_info_t** new_program)
        {
            const byte* pc = program->begin;
            while (pc < program->end)
            {
                const opcode_t opcode = *(opcode_t*)pc;
                pc += sizeof(opcode_t);

                switch (opcode)
                {
                    case OPCODE_JUMP:
                    {
                        jump_t* j    = (jump_t*)pc;
                        *new_program = j->m_program;
                    }
                    break;

                    case OPCODE_RUN:
                    {
                        run_t* r = (run_t*)pc;
                        execute_program(r->m_program, state, new_program);
                        return;
                    }

                    case OPCODE_ONCE:
                    {
                        once_t* op = (once_t*)pc;
                        pc += sizeof(once_t);

                        if (op->m_executed == 0)
                        {
                            const result_t result = op->m_function(state);
                            op->m_executed        = 1;

                            // TODO do we need to handle the result?
                        }
                    }

                    break;

                    case OPCODE_IF_FUNC:
                    {
                        opcode_if_func_t* op = (opcode_if_func_t*)pc;
                        pc += sizeof(opcode_if_func_t);

                        const result_t result = op->m_function(state);
                        if (result == RESULT_OK)
                        {
                            // condition not met, so we skip to the end of this if
                            pc = op->m_end;
                        }
                        else if (result == RESULT_DONE)
                        {
                            // condition met, we continue execution
                        }
                        else if (result == RESULT_ERROR)
                        {
                            // error ??
                        }
                    }

                    break;

                    case OPCODE_IF_TIMEOUT:
                    {
                        opcode_if_timeout_t* op = (opcode_if_timeout_t*)pc;
                        pc += sizeof(opcode_if_timeout_t);

                        const u64 currentTime = ntimer::millis();
                        if (op->m_startTime == 0)
                        {
                            op->m_startTime = currentTime;
                        }
                        else if (currentTime - op->m_startTime >= op->m_timeout)
                        {
                            // timeout reached, we continue execution of the scope
                        }
                        else
                        {
                            // timeout not yet reached, so we skip to the end of this if
                            pc = op->m_end;
                        }
                    }

                    break;

                    case OPCODE_RET:
                    {
                        return;
                    }

                }  // switch
            }  // while

            // we reached the end of the program, so we exit
        }

        void scheduler_t::tick(state_t* state)
        {
            if (m_currentProgram == nullptr)
                return;

            program_info_t* nextProgram = nullptr;
            execute_program(m_currentProgram, state, &nextProgram);

            if (nextProgram != nullptr)
            {
                reset_program(nextProgram);
                m_currentProgram = nextProgram;
            }
        }

    }  // namespace ntask
}  // namespace ncore