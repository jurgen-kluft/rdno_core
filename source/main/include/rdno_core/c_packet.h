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

        // Note: Little Endian byte order
        // Packet
        // {
        //     u8 length;       // Number of u32 in the packet
        //     u8 version;      // Packet version (currently 1)
        //
        //     // sensor value 1
        //     u8 id;             // stream id
        //     u16 value;         // Sensor value
        //
        //     // sensor value 2
        //     u8 id;             // stream id
        //     u16 value;         // Sensor value
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
            void write_sensor(u8 id, u16 value);
            s32  finalize();  // returns the number of sensor values written
        };

    }  // namespace npacket
}  // namespace ncore

#endif  // __RDNO_CORE_PACKET_H__
