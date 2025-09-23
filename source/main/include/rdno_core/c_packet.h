#ifndef __RDNO_CORE_PACKET_H__
#define __RDNO_CORE_PACKET_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace npacket
    {
        namespace nstate
        {
            typedef u8    value_t;
            const value_t Off   = 0x10;
            const value_t On    = 0x20;
            const value_t Error = 0x30;
        }  // namespace nstate

        namespace ntype
        {
            typedef u8    value_t;
            const value_t Unknown     = 0x00;
            const value_t Temperature = 0x01;  // (s8, °C)
            const value_t Humidity    = 0x02;  // (u8, %)
            const value_t Pressure    = 0x03;  // (u16, hPa)
            const value_t Light       = 0x04;  // (u16, lux)
            const value_t CO2         = 0x05;  // (u16, ppm)
            const value_t VOC         = 0x06;  // (u16, ppm)
            const value_t PM1_0       = 0x07;  // (u16, µg/m3)
            const value_t PM2_5       = 0x08;  // (u16, µg/m3)
            const value_t PM10        = 0x09;  // (u16, µg/m3)
            const value_t Noise       = 0x0A;  // (u8, dB)
            const value_t Presence    = 0x0B;  // (u8, 0=none, 1-8=target ID)
            const value_t Distance    = 0x0C;  // (u16, cm)
            const value_t UV          = 0x0D;  // (u8, index)
            const value_t CO          = 0x0E;  // (u16, ppm/10)
            const value_t Vibration   = 0x0F;  // (u8, 0=none, 1=low, 2=medium, 3=high)
            const value_t State       = 0x10;  // (u16 (u8[2]), sensor model, sensor state)
            const value_t MacAddress  = 0x11;  // (u64 (u8[8]), MAC address)
        };  // namespace ntype

        namespace nfieldtype
        {
            typedef s8    value_t;
            const value_t TypeNone = 0x00;
            const value_t TypeBit  = 0x01;
            const value_t TypeS8   = 0x08;
            const value_t TypeS16  = 0x10;
            const value_t TypeS32  = 0x20;
            const value_t TypeS64  = 0x40;
            const value_t TypeU8   = 0x88;
            const value_t TypeU16  = 0x90;
            const value_t TypeU32  = 0xA0;
            const value_t TypeU64  = 0xC0;

            inline value_t from_type(const ntype::value_t type)
            {
                switch (type)
                {
                    case ntype::Temperature: return TypeS8;
                    case ntype::Humidity: return TypeS8;
                    case ntype::Pressure: return TypeS16;
                    case ntype::Light: return TypeS16;
                    case ntype::CO2: return TypeS16;
                    case ntype::VOC: return TypeS16;
                    case ntype::PM1_0: return TypeS16;
                    case ntype::PM2_5: return TypeS16;
                    case ntype::PM10: return TypeS16;
                    case ntype::Noise: return TypeS8;
                    case ntype::Presence: return TypeS8;
                    case ntype::Distance: return TypeS16;
                    case ntype::UV: return TypeS8;
                    case ntype::CO: return TypeS8;
                    case ntype::Vibration: return TypeS8;
                    case ntype::State: return TypeS32;
                    case ntype::MacAddress: return TypeU64;
                    default: return TypeNone;
                }
            }
        }  // namespace nfieldtype

        // Note: Little Endian byte order
        // Packet
        // {
        //     u8 length;       // Number of u32 in the packet
        //     u8 version;      // Packet version (currently 1)
        //     u16 id;          // Fixed Id 0xA5C3
        //     u8 timesync[4];  // Time sync of the packet (bit 31 indicates if this packet is a time sync packet)
        //
        //     // sensor value 1
        //     u8 type;           // ntype (also implies sensor field type)
        //     s8|s16|s32 value;  // Sensor value (size depends on sensor type)
        //
        //     // sensor value 2
        //     u8 type;           // ntype (also implies sensor field type)
        //     s8|s16|s32 value;  // Sensor value (size depends on sensor type)
        //
        //     ...
        //
        //     Padding to align packet size to 2 bytes
        // };

        struct packet_t
        {
            byte Data[512];
            s32  Size;
            s32  Capacity;

            enum
            {
                // Packet header
                HeaderSize    = 1 + 1 + 2 + 4,  // length, version, id, time-sync
                LengthOffset  = 0,
                VersionOffset = 1,
                IdOffset      = 2,
                TimeOffset    = 4,
            };

            // Periodically it is recommended to send a packet with the time_sync flag set to true
            // This allows the receiver to resync its reference time with the sender.
            // Note: Upon connecting to the remote, the first packet should always have time_sync=true
            void begin(u32 time_ms, bool time_sync = true);
            void write_value(ntype::value_t type, u64 value);
            s32  finalize();  // returns the number of sensor values written
        };

    }  // namespace npacket
}  // namespace ncore

#endif  // __RDNO_CORE_PACKET_H__
