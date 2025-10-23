#ifndef __RDNO_CORE_TASK_H__
#define __RDNO_CORE_TASK_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_config.h"

namespace ncore
{
    struct state_app_t;
    struct state_wifi_t;
    struct state_tcp_t;
    struct state_udp_t;

    namespace ntask
    {
        struct state_t;

        typedef s16 result_t;
        typedef result_t (*function_t)(state_t*);

        const result_t RESULT_OK    = 0;
        const result_t RESULT_DONE  = 1;
        const result_t RESULT_ERROR = -1;

        struct timeout_t
        {
            timeout_t(u32 timeout_ms)
                : m_timeout_ms(timeout_ms)
            {
            }
            u32 m_timeout_ms;
        };

        struct program_info_t;
        typedef program_info_t* program_t;

        struct executor_t;

        executor_t* init(s32 max_programs, s32 program_mem_size = 2048);
        void        print_info(executor_t* exec);
        void        boot(executor_t* exec, program_t program);
        void        tick(executor_t* exec, state_t* state);

        program_t program(executor_t* exec, const char* name);
        void      op_begin(executor_t* exec, program_t program);
        void      op_jump(executor_t* exec, program_t program);
        void      op_run(executor_t* exec, program_t program);
        void      op_run_periodic(executor_t* exec, function_t fn, u32 period_ms);
        void      op_once(executor_t* exec, function_t fn);
        void      op_if(executor_t* exec, function_t fn);
        void      op_if(executor_t* exec, timeout_t timeout);
        void      op_return(executor_t* exec);
        void      op_end(executor_t* exec);

        struct state_t
        {
            nconfig::config_t* config;
            u64                time_ms;
            state_wifi_t*      wifi;
            state_tcp_t*       tcp;
            state_udp_t*       udp;
            state_app_t*       app;
            u32                flags;

            void set_config(bool valid) { flags = (flags & ~0x1) | (valid ? 1 : 0); }
            bool has_config() const { return (flags & 0x1) != 0; }
        };

    }  // namespace ntask
}  // namespace ncore

#endif  // __RDNO_CORE_TASK_H__