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
        void scheduler_t::reset() { m_counter = 0; }

        bool is_first_call(scheduler_t* scheduler) { return scheduler->m_counter == 0; }

        void init_timeout(scheduler_t* scheduler, timeout_t& timeout)
        {
            timeout.m_start_time_ms = scheduler->m_state_task->m_current_ms;
        }

        bool timeout(scheduler_t* scheduler, timeout_t& timeout)
        {
            const u64 current_time = scheduler->m_state_task->m_current_ms;
            return (current_time - timeout.m_start_time_ms) >= timeout.m_timeout_ms;
        }

        void init_periodic(scheduler_t* scheduler, periodic_t& periodic)
        {
            periodic.m_start_time_ms = scheduler->m_state_task->m_current_ms;;
        }

        bool periodic(scheduler_t* scheduler, periodic_t& periodic)
        {
            const u64 current_time = scheduler->m_state_task->m_current_ms;;
            if ((current_time - periodic.m_start_time_ms) >= periodic.m_period_ms)
            {
                periodic.m_start_time_ms += periodic.m_period_ms;
                if ((current_time - periodic.m_start_time_ms) >= periodic.m_period_ms)
                {
                    // If the gap is too large, hard reset to the current time
                    periodic.m_start_time_ms = current_time;
                }
                return true;
            }
            return false;
        }

        bool call(scheduler_t* scheduler, function_t func)
        {
            if (func != nullptr)
            {
                return func(scheduler->m_state) == RESULT_DONE;
            }
            return false;
        }

        void call_program(program_t* program)
        {
            if (program != nullptr && program->m_program != nullptr)
            {
                scheduler_t* scheduler = &program->m_scheduler;
                program->m_program(scheduler, scheduler->m_state);
                scheduler->m_counter++;
            }
        }

        void jmp_program(scheduler_t* scheduler, program_t* program)
        {
            if (program != nullptr)
            {
                scheduler->m_state_task->m_current_program = program;
                scheduler->m_state_task->m_current_program->m_scheduler.reset();
            }
        }

        void set_main(state_t* state, state_task_t* task_state, program_t* main_program) { task_state->m_main_program = main_program; }

        void set_start(state_t* state, state_task_t* task_state, program_t* start_program)
        {
            task_state->m_current_program = start_program;
            task_state->m_current_program->m_scheduler.reset();
        }

        void tick(state_t* state, state_task_t* task_state) 
        { 
            task_state->m_current_ms = ntimer::millis();
            call_program(task_state->m_current_program); 
        }

    }  // namespace ntask
}  // namespace ncore