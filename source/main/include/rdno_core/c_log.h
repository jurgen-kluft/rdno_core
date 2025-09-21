#ifndef __RDNO_CORE_LOG_H__
#define __RDNO_CORE_LOG_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_network.h"

namespace ncore
{
    namespace nlog
    {
        namespace nlog_level
        {
            typedef s8 value_t;

            const value_t Off   = 0;
            const value_t Fatal = 1;
            const value_t Error = 2;
            const value_t Warn  = 4;
            const value_t Info  = 8;
            const value_t Debug = 16;
            const value_t Trace = 32;
            const value_t All   = Fatal | Error | Warn | Info | Debug | Trace;
        }

        void set_level(nlog_level::value_t level);
        
        void log_fatal(const char* msg);
        void log_error(const char* msg);
        void log_warn(const char* msg);
        void log_info(const char* msg);
        void log_debug(const char* msg);
        void log_trace(const char* msg);
        void log_flush();

    }  // namespace nserial

}  // namespace ncore

#endif  // __RDNO_CORE_LOG_H__
