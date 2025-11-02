#ifndef __RDNO_CORE_TASK_H__
#define __RDNO_CORE_TASK_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_state.h"

namespace ncore
{
    struct state_task_t;

    namespace ntask
    {
        typedef s16 result_t;
        typedef result_t (*function_t)(state_t*);

        const result_t RESULT_OK    = 0;
        const result_t RESULT_DONE  = 1;
        const result_t RESULT_ERROR = -1;

        struct timeout_t
        {
            timeout_t(u32 timeout_ms)
                : m_timeout_ms(timeout_ms)
                , m_start_time_ms(0)
            {
            }
            u32 m_timeout_ms;
            u64 m_start_time_ms;
        };

        struct periodic_t
        {
            periodic_t(u32 period_ms)
                : m_period_ms(period_ms)
                , m_start_time_ms(0)
            {
            }
            u32 m_period_ms;
            u64 m_start_time_ms;
        };

        struct scheduler_t
        {
            scheduler_t()
                : m_counter(0)
                , m_state_task(nullptr)
            {
            }
            s32           m_counter;
            state_task_t* m_state_task;
            void          reset();
        };

        typedef void (*program_entry_t)(scheduler_t*, state_t*);

        struct program_t
        {
            program_t(program_entry_t entry)
                : m_program(entry)
            {
            }
            program_entry_t m_program;
            scheduler_t     m_scheduler;
        };

        bool is_first_call(scheduler_t* scheduler);
        void init_timeout(scheduler_t* scheduler, timeout_t& timeout);
        bool timeout(scheduler_t* scheduler, timeout_t& timeout);
        void init_periodic(scheduler_t* scheduler, periodic_t& periodic);
        bool periodic(scheduler_t* scheduler, periodic_t& periodic);
        bool call(scheduler_t* scheduler, function_t func);
        void call_program(scheduler_t* scheduler, program_t* program);
        void jmp_program(scheduler_t* scheduler, program_t* program);

        void set_main(state_t* state, state_task_t* task_state, program_t* main_program);
        void set_start(state_t* state, state_task_t* task_state, program_t* start_program);

        void tick(state_t* state, state_task_t* task_state);
    }  // namespace ntask

    struct state_task_t
    {
        u64                m_current_ms;
        state_t*           m_state;
        ntask::program_t*  m_main_program;
        ntask::program_t*  m_current_program;
    };

}  // namespace ncore

#endif  // __RDNO_CORE_TASK_H__