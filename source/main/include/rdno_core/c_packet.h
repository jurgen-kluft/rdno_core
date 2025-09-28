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

        namespace nfieldtype
        {
            typedef u8    field_t;
            const field_t TypeNone = 0x00;
            const field_t TypeBit  = 0x01;
            const field_t TypeS8   = 0x08;
            const field_t TypeS16  = 0x10;
            const field_t TypeS32  = 0x20;
            const field_t TypeS64  = 0x40;
            const field_t TypeU8   = 0x88;
            const field_t TypeU16  = 0x90;
            const field_t TypeU32  = 0xA0;
            const field_t TypeU64  = 0xC0;

            inline s32     size_in_bytes(field_t f) { return (f & 0x7F) >> 3; }
        }  // namespace nfieldtype

        // Note: Little Endian byte order
        // Packet
        // {
        //     u8 length;       // Number of u32 in the packet
        //     u8 version;      // Packet version (currently 1)
        //
        //     // sensor value 1
        //     u8 id;             // stream id
        //     s8|s16|s32 value;  // Sensor value (size depends on sensor type)
        //
        //     // sensor value 2
        //     u8 id;             // stream id
        //     s8|s16|s32 value;  // Sensor value (size depends on sensor type)
        //
        //     ...
        //
        //     Padding to align packet size to 2 bytes
        // };

        struct packet_t
        {
            byte Data[64];
            s32  Size;
            s32  Capacity;

            enum
            {
                // Packet header
                HeaderSize    = 1 + 1,  // length, version
                LengthOffset  = 0,
                VersionOffset = 1,
            };

            void begin();
            void write_sensor(u8 id, nfieldtype::field_t field, u64 value);
            s32  finalize();  // returns the number of sensor values written
        };

    }  // namespace npacket
}  // namespace ncore

#endif  // __RDNO_CORE_PACKET_H__
