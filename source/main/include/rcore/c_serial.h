#ifndef __RCORE_SERIAL_H__
#define __RCORE_SERIAL_H__
#include "rcore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_printf.h"

namespace ncore
{
    class reader_t;
    
    namespace nbaud
    {
        enum Enum
        {
            Rate300    = 300,
            Rate600    = 600,
            Rate1200   = 1200,
            Rate2400   = 2400,
            Rate4800   = 4800,
            Rate9600   = 9600,
            Rate14400  = 14400,
            Rate19200  = 19200,
            Rate28800  = 28800,
            Rate38400  = 38400,
            Rate57600  = 57600,
            Rate74880  = 74880,
            Rate115200 = 115200,
            Rate256000 = 256000,
        };
    }
    namespace nconfig
    {
        enum Enum
        {
            MODE_8N1 = 0x800001c,
        };
    }

    namespace nserialx
    {
        typedef u8 serial_t;

        const serial_t SERIAL0 = 0;
        const serial_t SERIAL1 = 1;
        const serial_t SERIAL2 = 2;
        const serial_t SERIAL3 = 3;
    }

    namespace nserial
    {
        void begin(nbaud::Enum baud = nbaud::Rate115200);
        void print(const char* val);
        void print(u8 a, u8 b, u8 c, u8 d, char separator = '.');
        void print(u8 a, u8 b, u8 c, u8 d, u8 e, u8 f, char separator = ':');
        void println(const char* val);

        template <typename... Args>
        void printf(const char* format, Args... args)
        {
            char       buffer[256];
            const va_t argv[] = {args...};
            const s32  argc   = sizeof(argv) / sizeof(argv[0]);
            snprintf_(buffer, sizeof(buffer) - 1, format, argv, argc);
            print(buffer);
        }

        template <typename... Args>
        void printfln(const char* format, Args... args)
        {
            char       buffer[256];
            const va_t argv[] = {args...};
            const s32  argc   = sizeof(argv) / sizeof(argv[0]);
            snprintf_(buffer, sizeof(buffer) - 1, format, argv, argc);
            println(buffer);
        }
    }  // namespace nserial

    namespace nserialx
    {
        void      begin(serial_t x, nbaud::Enum baud, nconfig::Enum config, s8 rxPin, s8 txPin);
        reader_t* reader(serial_t x);
        s32       available(serial_t x);
        void      print(serial_t x, const char* val);
        void      println(serial_t x, const char* val);
        void      write(serial_t x, const byte* data, s32 length);
        s32       read_until(serial_t x, char terminator, char* outString, s32 outMaxLength);
        s32       read_bytes(serial_t x, byte* outData, s32 outMaxLength);
    }  // namespace nserialx

}  // namespace ncore

#endif  // __RCORE_SERIAL_H__
