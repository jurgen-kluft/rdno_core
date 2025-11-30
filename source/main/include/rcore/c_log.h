#ifndef __RCORE_LOG_H__
#define __RCORE_LOG_H__
#include "rcore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_va_list.h"

namespace ncore
{
    namespace nlog
    {
        namespace nlevel
        {
            typedef u8 value_t;

            const value_t Off   = 0;
            const value_t Fatal = 1;
            const value_t Error = 2;
            const value_t Warn  = 4;
            const value_t Info  = 8;
            const value_t Debug = 16;
            const value_t Trace = 32;
            const value_t All   = Fatal | Error | Warn | Info | Debug | Trace;
        }  // namespace nlevel

        void set_level(nlevel::value_t level);

        void println(const char* msg);
        void println();

        void printf_(const char* format, va_t* args, i32 argc);
        template <typename... Args>
        inline void printf(const char* format, Args&&... _args)
        {
            const va_t argv[] = {_args...};
            const i32  argc   = sizeof(argv) / sizeof(argv[0]);
            printf_(format, (va_t*)argv, argc);
        }
        template <typename... Args>
        inline void printfln(const char* format, Args&&... _args)
        {
            const va_t argv[] = {_args...};
            const i32  argc   = sizeof(argv) / sizeof(argv[0]);
            printf_(format, (va_t*)argv, argc);
            println();
        }

        void fatal(const char* msg);
        void error(const char* msg);
        void warn(const char* msg);
        void info(const char* msg);
        void debug(const char* msg);
        void trace(const char* msg);
        void flush();

        // Mac
        void print_mac(const u8* mac);
        inline void println_mac(const u8* mac)
        {
            print_mac(mac);
            println();
        }

    }  // namespace nlog

}  // namespace ncore

#endif  // __RCORE_LOG_H__
