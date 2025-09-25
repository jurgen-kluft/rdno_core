#ifndef __RDNO_CORE_TASK_H__
#define __RDNO_CORE_TASK_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_config.h"

namespace ncore
{
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

        struct scheduler_t
        {
            byte* m_program_mem;
            byte* m_scope_open;
            byte* m_cursor;
            s32   m_scope;

            program_t m_currentProgram;

            void initialize();
            void tick(state_t* state);

            program_t program();
            void      xbegin(program_t program);
            void      xjump(program_t program);
            void      xrun(program_t program);
            void      xonce(function_t fn);
            void      xif(function_t fn);
            void      xif(timeout_t timeout);
            void      xreturn();
            void      xend();
        };

        struct state_wifi_t;
        struct state_remote_t;
        struct state_app_t;
        struct state_t
        {
            nconfig::config_t* config;
            state_wifi_t*      wifi;
            state_remote_t*    remote;
            state_app_t*       app;
        };

    }  // namespace ntask
}  // namespace ncore

#endif  // __RDNO_CORE_TASK_H__