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
            const Value Unknown     = 0x00;
            const Value Temperature = 0x01;  // (s8, °C)
            const Value Humidity    = 0x02;  // (u8, %)
            const Value Pressure    = 0x03;  // (u16, hPa)
            const Value Light       = 0x04;  // (u16, lux)
            const Value CO2         = 0x05;  // (u16, ppm)
            const Value VOC         = 0x06;  // (u16, ppm)
            const Value PM1_0       = 0x07;  // (u16, µg/m3)
            const Value PM2_5       = 0x08;  // (u16, µg/m3)
            const Value PM10        = 0x09;  // (u16, µg/m3)
            const Value Noise       = 0x0A;  // (u8, dB)
            const Value Presence    = 0x0B;  // (u8, 0=none, 1-8=target ID)
            const Value Distance    = 0x0C;  // (u16, cm)
            const Value UV          = 0x0D;  // (u8, index)
            const Value CO          = 0x0E;  // (u16, ppm/10)
            const Value Vibration   = 0x0F;  // (u8, 0=none, 1=low, 2=medium, 3=high)
            const Value State       = 0x10;  // (u16 (u8[2]), sensor model, sensor state)
        };  // namespace SensorType

        namespace SensorFieldType
        {
            typedef s8  Value;
            const Value TypeNone = 0x00;
            const Value TypeS8   = 0x08;
            const Value TypeS16  = 0x10;
            const Value TypeS32  = 0x20;
        }  // namespace SensorFieldType

        inline SensorFieldType::Value ToSensorFieldType(const SensorType::Value type)
        {
            switch (type)
            {
                case SensorType::Temperature: return SensorFieldType::TypeS8;
                case SensorType::Humidity: return SensorFieldType::TypeS8;
                case SensorType::Pressure: return SensorFieldType::TypeS16;
                case SensorType::Light: return SensorFieldType::TypeS16;
                case SensorType::CO2: return SensorFieldType::TypeS16;
                case SensorType::VOC: return SensorFieldType::TypeS16;
                case SensorType::PM1_0: return SensorFieldType::TypeS16;
                case SensorType::PM2_5: return SensorFieldType::TypeS16;
                case SensorType::PM10: return SensorFieldType::TypeS16;
                case SensorType::Noise: return SensorFieldType::TypeS8;
                case SensorType::Presence: return SensorFieldType::TypeS8;
                case SensorType::Distance: return SensorFieldType::TypeS16;
                case SensorType::UV: return SensorFieldType::TypeS8;
                case SensorType::CO: return SensorFieldType::TypeS8;
                case SensorType::Vibration: return SensorFieldType::TypeS8;
                case SensorType::State: return SensorFieldType::TypeS32;
                default: return SensorFieldType::TypeNone;
            }
        }

        // Note: Little Endian byte order
        // Packet structure
        // {
        //     u8 length;       // Number of u32 in the packet
        //     u8 version;      // Packet version (currently 1)
        //     u8 timesync[4];  // Time sync of the packet (bit 31 indicates if this packet is a time sync packet)
        //
        //     // sensor value 1
        //     u8 type;           // SensorType (also implies sensor field type)
        //     s8|s16|s32 value;  // Sensor value (size depends on sensor type)
        //  
        //     // sensor value 2
        //     u8 type;           // SensorType (also implies sensor field type)
        //     s8|s16|s32 value;  // Sensor value (size depends on sensor type)
        //
        //     ...
        //
        //     Padding to align packet size to 2 bytes
        // };

        const u8 SensorPacket_Version = 1;

        struct SensorPacket_t
        {
            byte Data[512];
            s32  Size;
            s32  Capacity;

            enum
            {
                // Packet header
                HeaderSize    = 1 + 1 + 4,  // length, version, time-sync
                LengthOffset  = 0,
                VersionOffset = 1,
                TimeOffset    = 2,
            };

            // Periodically it is recommended to send a packet with the time_sync flag set to true
            // This allows the receiver to resync its reference time with the sender.
            // Note: Upon connecting to the remote, the first packet should always have time_sync=true
            void begin(u32 time_ms, bool time_sync = true);
            s32  finalize();  // returns the number of sensor values written

            void write_sensor_value(SensorType::Value type, s32 value);
        };

    }  // namespace nsensor
}  // namespace ncore

#endif  // __RDNO_CORE_SENSOR_PACKET_H__
