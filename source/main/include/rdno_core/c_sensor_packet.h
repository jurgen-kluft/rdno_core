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
            typedef u8  Value;
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
            // Note: Max 15 locations
            const Value Location15 = 0x0F;

            const Value Bedroom    = 0x10;
            const Value Livingroom = 0x20;
            const Value Kitchen    = 0x30;
            const Value Bathroom   = 0x40;
            const Value Hallway    = 0x50;
            const Value Balcony    = 0x60;
            const Value Study      = 0x70;
            const Value Pantry     = 0x80;
            // Note: Max 15 rooms
        }  // namespace DeviceLocation

        namespace SensorModel
        {
            typedef u8  Value;
            const Value GPIO   = 0x00;  // Generic GPIO sensor
            const Value BH1750 = 0x10;  // Light sensor
            const Value BME280 = 0x20;  // Temperature, Humidity, Pressure sensor
            const Value SCD4X  = 0x30;  // CO2, Temperature, Humidity sensor
            const Value HMMD   = 0x40;  // Presence (Doppler radar) sensor
        };  // namespace SensorModel

        namespace SensorState
        {
            typedef u8  Value;
            const Value Off   = 0x10;
            const Value On    = 0x20;
            const Value Error = 0x30;
        }  // namespace SensorState

        namespace SensorType
        {
            typedef u8  Value;
            const Value Temperature = 0x00;  // (s8, °C)
            const Value Humidity    = 0x01;  // (u8, %)
            const Value Pressure    = 0x02;  // (u16, hPa)
            const Value Light       = 0x03;  // (u16, lux)
            const Value CO2         = 0x04;  // (u16, ppm)
            const Value VOC         = 0x05;  // (u16, ppm)
            const Value PM1_0       = 0x06;  // (u16, µg/m3)
            const Value PM2_5       = 0x07;  // (u16, µg/m3)
            const Value PM10        = 0x08;  // (u16, µg/m3)
            const Value Noise       = 0x09;  // (u8, dB)
            const Value Presence    = 0x0A;  // (u8, 0=none, 1-8=target ID)
            const Value Distance    = 0x0B;  // (u16, cm)
            const Value UV          = 0x0C;  // (u8, index)
            const Value CO          = 0x0D;  // (u16, ppm/10)
            const Value Vibration   = 0x0E;  // (u8, 0=none, 1=low, 2=medium, 3=high)
            const Value State       = 0x0F;  // (u16 (u8[2]), sensor model, sensor state)
        };  // namespace SensorType

        namespace SensorFieldType
        {
            typedef s8  Value;
            const Value TypeNone = 0x00;
            const Value TypeS8   = 0x01;
            const Value TypeS16  = 0x02;
            const Value TypeS32  = 0x03;
            const Value TypeU8   = 0x04;
            const Value TypeU16  = 0x05;
            const Value TypeU32  = 0x06;
        }  // namespace SensorFieldType

        inline SensorFieldType::Value ToSensorFieldType(const SensorType::Value type)
        {
            switch (type)
            {
                case SensorType::Temperature: return SensorFieldType::TypeS8;
                case SensorType::Humidity: return SensorFieldType::TypeU8;
                case SensorType::Pressure: return SensorFieldType::TypeU16;
                case SensorType::Light: return SensorFieldType::TypeU16;
                case SensorType::CO2: return SensorFieldType::TypeU16;
                case SensorType::VOC: return SensorFieldType::TypeU16;
                case SensorType::PM1_0: return SensorFieldType::TypeU16;
                case SensorType::PM2_5: return SensorFieldType::TypeU16;
                case SensorType::PM10: return SensorFieldType::TypeU16;
                case SensorType::Noise: return SensorFieldType::TypeU8;
                case SensorType::Presence: return SensorFieldType::TypeU8;
                case SensorType::Distance: return SensorFieldType::TypeU16;
                case SensorType::UV: return SensorFieldType::TypeU8;
                case SensorType::CO: return SensorFieldType::TypeU8;
                case SensorType::Vibration: return SensorFieldType::TypeU8;
                case SensorType::State: return SensorFieldType::TypeU16;  // Note: 2 bytes (model and state)
                default: return SensorFieldType::TypeNone;
            }
        }

        // Note: Little Endian byte order
        // Packet structure
        // {
        //     u8                    length;   // Number of u32 in the packet
        //     u8                    sequence; // Sequence number of the packet
        //     u8                    version;  // Version of the packet structure
        //     DeviceLocation::Value location;
        //     // sensor value 1
        //     u8 type;                        // SensorType (also implies sensor field type)
        //     One of the following:
        //         - s8  s8_value;
        //         - u8  u8_value;
        //         - s16 s16_value;
        //         - u16 u16_value;
        //         - s32 s32_value;
        //         - u32 u32_value;
        //     // sensor value 2
        //     // ... 
        //     Padding to align packet size to 4 bytes
        // };

        struct SensorPacket_t
        {
            byte Data[256];
            s32  Size;
            s32  Capacity;

            enum
            {
                // Packet header
                HeaderSize     = 1 + 1 + 1 + 1,  // length, sequence, version, location
                LengthOffset   = 0,
                SequenceOffset = 1,
                VersionOffset  = 2,
                LocationOffset = 3,
            };

            void begin(u8 sequence, u8 version, DeviceLocation::Value location);
            s32  finalize();  // returns the number of sensor values written

            void write_sensor_value(SensorType::Value type, s32 value);
            void write_sensor_value(SensorType::Value type, u32 value);
        };

    }  // namespace nsensor
}  // namespace ncore

#endif  // __RDNO_CORE_SENSOR_PACKET_H__
