#include "rdno_core/c_log.h"
#include "rdno_core/c_timer.h"
#include "rdno_core/c_str.h"

#ifdef TARGET_ESP32

#    include "Arduino.h"

namespace ncore
{
    namespace nlog
    {
        // Note disable all logging in final builds
        nlog_level::value_t gLogLevel = nlog_level::All;

        char                gLogLineBuffer[256];
        str_t               gLogLineStr   = {gLogLineBuffer, gLogLineBuffer, 0, 0, (s16)sizeof(gLogLineBuffer) - 1};
        nlog_level::value_t gLogLineLevel = nlog_level::Info;

        void set_level(nlog_level::value_t level) { gLogLevel = level; }

        static inline bool is_level_active(nlog_level::value_t level) { return (gLogLevel & level) != 0; }

        void log_flush()
        {
            if (gLogLineLevel != nlog_level::Off)
            {
                Serial.println(gLogLineBuffer);
                gLogLineBuffer[0] = 0;
                gLogLineStr.m_str = 0;
                gLogLineStr.m_end = 0;
                gLogLineLevel     = nlog_level::Off;
            }
        }

        void log_fatal(const char* msg) 
        { 
            log_flush(); 
            gLogLineLevel = nlog_level::Fatal;
            str_append(gLogLineStr, "[FATAL] ");
            str_append(gLogLineStr, msg);
        }
        void log_error(const char* msg)
        {
            log_flush(); 
            gLogLineLevel = nlog_level::Error;
            str_append(gLogLineStr, "[ERROR] ");
            str_append(gLogLineStr, msg);
        }
        void log_warn(const char* msg)
        {
            log_flush();
            gLogLineLevel = nlog_level::Warn;
            str_append(gLogLineStr, "[WARN ] ");
            str_append(gLogLineStr, msg);
        }
        void log_info(const char* msg)
        {
            log_flush();
            gLogLineLevel = nlog_level::Info;
            str_append(gLogLineStr, "[INFO ] ");
            str_append(gLogLineStr, msg);
        }
        void log_debug(const char* msg)
        {
            log_flush();
            gLogLineLevel = nlog_level::Debug;
            str_append(gLogLineStr, "[DEBUG] ");
            str_append(gLogLineStr, msg);
        }
        void log_trace(const char* msg)
        {
            log_flush();
            gLogLineLevel = nlog_level::Trace;
            str_append(gLogLineStr, "[TRACE] ");
            str_append(gLogLineStr, msg);
        }

    }  // namespace nlog

}  // namespace ncore

#else

namespace ncore
{
    namespace nlog
    {

    }  // namespace nlog

}  // namespace ncore

#endif
