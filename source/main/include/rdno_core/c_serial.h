#ifndef __RDNO_CORE_SERIAL_H__
#define __RDNO_CORE_SERIAL_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_network.h"

namespace ncore
{
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
            Rate115200 = 115200,
        };
    }
    namespace nconfig
    {
        enum Enum
        {
            MODE_8N1 = 0x800001c,
        };
    }

    namespace nserial
    {
        void begin(nbaud::Enum baud = nbaud::Rate115200);
        void print(const char* val);
        void print(const u8 val, bool hex = false);
        void print(const IPAddress_t& address);
        void print(const MACAddress_t& address);
        void println(const char* val);
    }  // namespace nserial

    namespace nserial1
    {
        void begin(nbaud::Enum baud, nconfig::Enum config, u8 rxPin, u8 txPin);
        s32  available();
        void print(const char* val);
        void println(const char* val);
        void write(const byte* data, s32 length);
        s32  read_until(char terminator, char* outString, s32 outMaxLength);
    }  // namespace nserial1

    namespace nserial2
    {
        void begin(nbaud::Enum baud, nconfig::Enum config, u8 rxPin, u8 txPin);
        s32  available();
        void print(const char* val);
        void println(const char* val);
        void write(const byte* data, s32 length);
        s32  read_until(char terminator, char* outString, s32 outMaxLength);
        s32  read_bytes(byte* outData, s32 outMaxLength);
    }  // namespace nserial2

}  // namespace ncore

#endif  // __RDNO_CORE_SERIAL_H__
