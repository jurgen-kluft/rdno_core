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
        namespace DeviceLocation
        {
            typedef u16 Value;
            const Value Unknown   = 0;
            const Value Location1 = 0x01;
            const Value Location2 = 0x02;
            const Value Location3 = 0x03;
            const Value Location4 = 0x04;
            const Value Location5 = 0x05;
            const Value Location6 = 0x06;
            const Value Location7 = 0x07;
            const Value Location8 = 0x08;
            const Value Location9 = 0x09;
            // Note: Max 15 rooms
            const Value Location15 = 0x0F;

            const Value Area1 = 0x10;
            const Value Area2 = 0x20;
            const Value Area3 = 0x30;
            const Value Area4 = 0x40;
            const Value Area5 = 0x50;
            const Value Area6 = 0x60;
            const Value Area7 = 0x70;
            const Value Area8 = 0x80;
            const Value Area9 = 0x90;
            // Note: Max 15 areas
            const Value Area15 = 0xF0;

            const Value Bedroom    = 0x0100;
            const Value Livingroom = 0x0200;
            const Value Kitchen    = 0x0300;
            const Value Bathroom   = 0x0400;
            const Value Hallway    = 0x0500;
            const Value Balcony    = 0x0600;
            const Value Study      = 0x0700;
            const Value Pantry     = 0x0800;
        }  // namespace DeviceLocation

        namespace SensorModel
        {
            typedef u8  Value;
            const Value GPIO = 0x0;
            const Value BH1750 = 0x10;
            const Value BME280 = 0x11;
            const Value SCD4X  = 0x12;
            const Value HMMD  = 0x13;
        };  // namespace SensorModel

        namespace SensorType
        {
            typedef u8  Value;
            const Value Temperature = 0x0;   // (float, °C)
            const Value Humidity    = 0x1;   // (float, %)
            const Value Pressure    = 0x2;   // (float, hPa)
            const Value Light       = 0x3;   // (float, lux)
            const Value CO2         = 0x4;   // (float, ppm)
            const Value VOC         = 0x5;   // (float, ppm)
            const Value PM1_0       = 0x6;   // (float, µg/m3)
            const Value PM2_5       = 0x7;   // (float, µg/m3)
            const Value PM10        = 0x8;   // (float, µg/m3)
            const Value Noise       = 0x9;   // (float, dB)
            const Value Presence    = 0x10;  // (float, 0.0-1.0)
            const Value Target      = 0x20;  // (channel index indicates X, Y, Z axis)
        };  // namespace SensorType

        namespace SensorState
        {
            typedef u8  Value;
            const Value Off   = 0x1;
            const Value On    = 0x2;
            const Value Error = 0x3;
        };  // namespace SensorState

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

        namespace DeviceLabel
        {
            typedef byte Value;
            const Value  Presence   = 0x01;
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
                HeaderSize        = 2 + 2 + 1 + 1 + 1 + 1,  // length, sequence, version, location, label, count
                LengthOffset      = 0,
                SensorCountOffset = 7,
            };

            void begin(u16 sequence, u8 version);

            void write_info(DeviceLocation::Value location, DeviceLabel::Value label);

            void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, s8 value);
            void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, s16 value);
            void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, s32 value);

            void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, u8 value);
            void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, u16 value);
            void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, u32 value);

            void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, f32 value);

            void finalize();
        };

    }  // namespace nsensor
}  // namespace ncore

#endif  // __RDNO_CORE_SENSOR_PACKET_H__
