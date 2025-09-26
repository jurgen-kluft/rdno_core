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
            typedef u16 type_t;
        }

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

            inline field_t from_type(const ntype::type_t type) { return (type >> 8) & 0xFF; }
            inline s32     size_in_bytes(field_t f) { return (f & 0x7F) >> 3; }
        }  // namespace nfieldtype

        namespace ntype
        {
            const type_t Unknown     = 0x00;
            const type_t Temperature = 0x01 | (nfieldtype::TypeS8 << 8);   // (s8, °C)
            const type_t Humidity    = 0x02 | (nfieldtype::TypeU8 << 8);   // (u8, %)
            const type_t Pressure    = 0x03 | (nfieldtype::TypeU16 << 8);  // (u16, hPa)
            const type_t Light       = 0x04 | (nfieldtype::TypeU16 << 8);  // (u16, lux)
            const type_t CO2         = 0x05 | (nfieldtype::TypeU16 << 8);  // (u16, ppm)
            const type_t VOC         = 0x06 | (nfieldtype::TypeU16 << 8);  // (u16, ppm)
            const type_t PM1_0       = 0x07 | (nfieldtype::TypeU16 << 8);  // (u16, µg/m3)
            const type_t PM2_5       = 0x08 | (nfieldtype::TypeU16 << 8);  // (u16, µg/m3)
            const type_t PM10        = 0x09 | (nfieldtype::TypeU16 << 8);  // (u16, µg/m3)
            const type_t Noise       = 0x0A | (nfieldtype::TypeU8 << 8);   // (u8, dB)
            const type_t UV          = 0x0B | (nfieldtype::TypeU8 << 8);   // (u8, index)
            const type_t CO          = 0x0C | (nfieldtype::TypeU8 << 8);   // (u16, ppm/10)
            const type_t Vibration   = 0x0D | (nfieldtype::TypeU8 << 8);   // (u8, 0=none, 1=low, 2=medium, 3=high)
            const type_t State       = 0x0E | (nfieldtype::TypeU16 << 8);  // (u16, state)
            const type_t MacAddress  = 0x0F | (nfieldtype::TypeU64 << 8);  // (u64 (u8[8]), MAC address)
            const type_t Presence1   = 0x10 | (nfieldtype::TypeU8 << 8);   // (u8, 0=none, 1=trigger up/down, 2=presence)
            const type_t Presence2   = 0x11 | (nfieldtype::TypeU8 << 8);   // (u8, 0=none, 1=trigger up/down, 2=presence)
            const type_t Presence3   = 0x12 | (nfieldtype::TypeU8 << 8);   // (u8, 0=none, 1=trigger up/down, 2=presence)
            const type_t Distance1   = 0x13 | (nfieldtype::TypeU16 << 8);  // (u16, cm)
            const type_t Distance2   = 0x14 | (nfieldtype::TypeU16 << 8);  // (u16, cm)
            const type_t Distance3   = 0x15 | (nfieldtype::TypeU16 << 8);  // (u16, cm)

            inline u8 type(const type_t type) { return type & 0xFF; }
            inline u8 field(const type_t type) { return (type >> 8) & 0xFF; }
        };  // namespace ntype

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
            byte Data[64];
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
            void write_value(ntype::type_t type, u64 value);
            s32  finalize();  // returns the number of sensor values written
        };

    }  // namespace npacket
}  // namespace ncore

#endif  // __RDNO_CORE_PACKET_H__
