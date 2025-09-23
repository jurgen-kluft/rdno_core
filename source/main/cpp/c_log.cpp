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
        nlevel::value_t gLogLevel = nlevel::All;

        char            gLogLineBuffer[256];
        str_t           gLogLineStr   = {gLogLineBuffer, gLogLineBuffer, 0, 0, (s16)sizeof(gLogLineBuffer) - 1};
        nlevel::value_t gLogLineLevel = nlevel::Info;

        void set_level(nlevel::value_t level) { gLogLevel = level; }

        static inline bool is_level_active(nlevel::value_t level) { return (gLogLevel & level) != 0; }

        void flush()
        {
            if (gLogLineLevel != nlevel::Off)
            {
                Serial.println(gLogLineBuffer);
                gLogLineBuffer[0] = 0;
                gLogLineStr.m_str = 0;
                gLogLineStr.m_end = 0;
                gLogLineLevel     = nlevel::Off;
            }
        }

        void fatal(const char* msg)
        {
            flush();
            gLogLineLevel = nlevel::Fatal;
            str_append(gLogLineStr, "[FATAL] ");
            str_append(gLogLineStr, msg);
        }
        void error(const char* msg)
        {
            flush();
            gLogLineLevel = nlevel::Error;
            str_append(gLogLineStr, "[ERROR] ");
            str_append(gLogLineStr, msg);
        }
        void warn(const char* msg)
        {
            flush();
            gLogLineLevel = nlevel::Warn;
            str_append(gLogLineStr, "[WARN ] ");
            str_append(gLogLineStr, msg);
        }
        void info(const char* msg)
        {
            flush();
            gLogLineLevel = nlevel::Info;
            str_append(gLogLineStr, "[INFO ] ");
            str_append(gLogLineStr, msg);
        }
        void debug(const char* msg)
        {
            flush();
            gLogLineLevel = nlevel::Debug;
            str_append(gLogLineStr, "[DEBUG] ");
            str_append(gLogLineStr, msg);
        }
        void trace(const char* msg)
        {
            flush();
            gLogLineLevel = nlevel::Trace;
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
