#include "rcore/c_log.h"
#include "rcore/c_str.h"
#include "ccore/c_printf.h"

#ifdef TARGET_ARDUINO
#    include "Arduino.h"

namespace ncore
{
    namespace nlog
    {
        // Note disable all logging in final builds
        char  gLogLineBuffer[256];
        str_t gLogLineStr = {gLogLineBuffer, gLogLineBuffer, 0, 0, (s16)sizeof(gLogLineBuffer) - 1};

        nlevel::value_t    gLogLevel = nlevel::All;
        void               set_level(nlevel::value_t level) { gLogLevel = level; }
        static inline bool is_level_active(nlevel::value_t level) { return (gLogLevel & level) != 0; }

        void flush()
        {
            Serial.println(gLogLineBuffer);
            gLogLineBuffer[0] = 0;
            gLogLineStr.m_str = 0;
            gLogLineStr.m_end = 0;
        }

        void fatal(const char* msg)
        {
            if (!is_level_active(nlevel::Fatal))
                return;
            str_append(gLogLineStr, "[FATAL] ");
            str_append(gLogLineStr, msg);
            flush();
        }

        void error(const char* msg)
        {
            if (!is_level_active(nlevel::Error))
                return;
            str_append(gLogLineStr, "[ERROR] ");
            str_append(gLogLineStr, msg);
            flush();
        }

        void warn(const char* msg)
        {
            if (!is_level_active(nlevel::Warn))
                return;
            str_append(gLogLineStr, "[WARN ] ");
            str_append(gLogLineStr, msg);
            flush();
        }

        void info(const char* msg)
        {
            if (!is_level_active(nlevel::Info))
                return;
            str_append(gLogLineStr, "[INFO ] ");
            str_append(gLogLineStr, msg);
            flush();
        }

        void debug(const char* msg)
        {
            if (!is_level_active(nlevel::Debug))
                return;
            str_append(gLogLineStr, "[DEBUG] ");
            str_append(gLogLineStr, msg);
            flush();
        }

        void trace(const char* msg)
        {
            if (!is_level_active(nlevel::Trace))
                return;
            str_append(gLogLineStr, "[TRACE] ");
            str_append(gLogLineStr, msg);
            flush();
        }

        void println(const char* msg)
        {
            if (!is_level_active(nlevel::Info))
                return;
            str_append(gLogLineStr, msg);
            flush();
        }

        void println()
        {
            if (!is_level_active(nlevel::Info))
                return;
            str_append(gLogLineStr, "\n");
            flush();
        }

        void print_mac(const u8* mac)
        {
            for (i32 i = 0; i < 6; ++i)
            {
                if (i > 0)
                    str_append(gLogLineStr, ":");
                to_str(gLogLineStr, (byte)mac[i], 2, 16);
            }
            flush();
        }

        void printf_(const char* format, va_t* args, i32 argc)
        {
            char*        str = gLogLineBuffer + gLogLineStr.m_str;
            const size_t n   = (size_t)sizeof(gLogLineBuffer) - gLogLineStr.m_str;
            const s32    len = ncore::snprintf_(str, n, format, args, argc);
            gLogLineStr.m_end += len;
            flush();
        }

    }  // namespace nlog

}  // namespace ncore

#else

namespace ncore
{
    namespace nlog
    {
        void set_level(nlevel::value_t level) { CC_UNUSED(level); }
        void println(const char* msg) { CC_UNUSED(msg); }
        void println() {}

        void printf_(const char* format, va_t* args, i32 argc)
        {
            CC_UNUSED(format);
            CC_UNUSED(args);
            CC_UNUSED(argc);
        }

        void fatal(const char* msg) { CC_UNUSED(msg); }
        void error(const char* msg) { CC_UNUSED(msg); }
        void warn(const char* msg) { CC_UNUSED(msg); }
        void info(const char* msg) { CC_UNUSED(msg); }
        void debug(const char* msg) { CC_UNUSED(msg); }
        void trace(const char* msg) { CC_UNUSED(msg); }
        void flush() {}

        // Mac
        void print_mac(const u8* mac) { CC_UNUSED(mac); }

    }  // namespace nlog

}  // namespace ncore

#endif
