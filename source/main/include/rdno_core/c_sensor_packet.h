#ifndef __RDNO_CORE_SENSOR_PACKET_H__
#define __RDNO_CORE_SENSOR_PACKET_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nsensor
    {
        namespace SensorState
        {
            typedef u8  Value;
            const Value Off   = 0x1;
            const Value On    = 0x2;
            const Value Error = 0x3;
        };  // namespace SensorState

        namespace SensorChannel
        {
            typedef u8  Value;
            const Value Channel0 = 0x0;
            const Value Channel1 = 0x1;
            const Value Channel2 = 0x2;
            const Value Channel3 = 0x3;
            const Value Channel4 = 0x4;
            const Value Channel5 = 0x5;
            const Value Channel6 = 0x6;
            const Value Channel7 = 0x7;
        };  // namespace SensorChannel

        namespace FieldType
        {
            typedef u8  Value;
            const Value TypeS8  = 0x0;
            const Value TypeS16 = 0x1;
            const Value TypeS32 = 0x2;
            const Value TypeU8  = 0x3;
            const Value TypeU16 = 0x4;
            const Value TypeU32 = 0x5;
            const Value TypeF32 = 0x6;
        };  // namespace FieldType

        namespace SensorType
        {
            typedef u8  Value;
            const Value Temperature = 0x0;  //
            const Value Humidity    = 0x1;  //
            const Value Pressure    = 0x2;  //
            const Value Light       = 0x3;  //
            const Value CO2         = 0x4;  //
            const Value Presence    = 0x5;  // (float, 0.0-1.0)
            const Value Target      = 0x6;  // (channel index indicates X, Y, Z axis)
        };  // namespace SensorType

        namespace DeviceLocation
        {
            typedef byte Value;
            const Value  Unknown     = 0;
            const Value  Bedroom1    = 1;
            const Value  Bedroom2    = 2;
            const Value  Bedroom3    = 3;
            const Value  Bedroom4    = 4;
            const Value  Livingroom1 = 5;
            const Value  Livingroom2 = 6;
            const Value  Livingroom3 = 7;
            const Value  Livingroom4 = 8;
            const Value  Kitchen1    = 9;
            const Value  Kitchen2    = 10;
            const Value  Kitchen3    = 11;
            const Value  Kitchen4    = 12;
            const Value  Bathroom1   = 13;
            const Value  Bathroom2   = 14;
            const Value  Bathroom3   = 15;
            const Value  Bathroom4   = 16;
            const Value  Hallway     = 17;
            const Value  ChildARoom  = 18;
            const Value  ChildBRoom  = 19;
            const Value  ChildCRoom  = 20;
            const Value  ChildDRoom  = 21;
            const Value  Guest1Room  = 22;
            const Value  Guest2Room  = 23;
            const Value  Study1Room  = 24;
            const Value  Study2Room  = 25;
            const Value  Balcony1    = 26;
            const Value  Balcony2    = 27;
            const Value  Balcony3    = 28;
            const Value  Balcony4    = 29;

        }  // namespace DeviceLocation

        namespace DeviceLabel
        {
            typedef byte Value;
            const Value  BedPresence = 0x01;
            const Value  AirQuality = 0x02;
        }  // namespace DeviceLabel

        // Note: Little Endian byte order

        // Packet structure
        // {
        //     u16                   length; // Number of bytes in the packet
        //     u16                   sequence; // Sequence number of the packet
        //     u8                    version; // Version of the packet structure
        //     DeviceLocation::Value location;
        //     DeviceLabel::Value    label;
        //     u8                    count;  // Number of sensor values in the packet (max 16)

        //     // sensor value 1
        //     u8 type_and_channel;
        //     u8 state_and_field_type;
        //     union
        //     {
        //         s8  s8_value;
        //         s16 s16_value;
        //         s32 s32_value;
        //         u8  u8_value;
        //         u16 u16_value;
        //         u32 u32_value;
        //         f32 f32_value;
        //     } value;

        //     // sensor value 2

        //     // ... (up to max 16 sensor values)

        //     // terminator, 2 bytes
        //     u16 terminator; // 0xFFFF
        // };

        struct SensorPacket_t
        {
            byte Data[128];
            s32  Size;
            s32  Capacity;

            enum
            {
                // Packet header
                HeaderSize      = 2 + 2 + 1 + 1 + 1 + 1,  // length, sequence, version, location, label, count
                LengthOffset      = 0,
                SensorCountOffset = 7,
            };

            void begin(u16 sequence, u8 version);

            void write_info(DeviceLocation::Value location, DeviceLabel::Value label);

            void write_sensor_value(SensorType::Value type, SensorChannel::Value channel, SensorState::Value state, s8 value);
            void write_sensor_value(SensorType::Value type, SensorChannel::Value channel, SensorState::Value state, s16 value);
            void write_sensor_value(SensorType::Value type, SensorChannel::Value channel, SensorState::Value state, s32 value);

            void write_sensor_value(SensorType::Value type, SensorChannel::Value channel, SensorState::Value state, u8 value);
            void write_sensor_value(SensorType::Value type, SensorChannel::Value channel, SensorState::Value state, u16 value);
            void write_sensor_value(SensorType::Value type, SensorChannel::Value channel, SensorState::Value state, u32 value);

            void write_sensor_value(SensorType::Value type, SensorChannel::Value channel, SensorState::Value state, f32 value);

            void finalize();
        };

    }  // namespace nsensor
}  // namespace ncore

#endif  // __RDNO_CORE_SENSOR_PACKET_H__
