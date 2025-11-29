#ifndef __rCORE_PACKET_H__
#define __rCORE_PACKET_H__
#include "rcore/c_target.h"
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
        //     u8 mac[6];       // MAC address of the node
        //
        //     // sensor value 1
        //     u8 id;             // sensor id (temperature, humidity, pressure, etc..)
        //     u16 value;         // Sensor value
        //
        //     // sensor value 2
        //     u8 id;             // sensor id
        //     u16 value;         // Sensor value
        //
        //     ...
        //
        //     Padding to align packet size to 2 bytes
        // };

        namespace nsensorid
        {
            typedef u8 value_t;
            enum
            {
                ID_UNKNOWN     = 0,   // Unknown
                ID_TEMPERATURE = 1,   // Temperature
                ID_HUMIDITY    = 2,   // Humidity
                ID_PRESSURE    = 3,   // Pressure
                ID_LIGHT       = 4,   // Light
                ID_UV          = 5,   // UV
                ID_CO          = 6,   // Carbon Monoxide
                ID_CO2         = 7,   // Carbon Dioxide
                ID_HCHO        = 8,   // Formaldehyde
                ID_VOC         = 9,   // Volatile Organic Compounds
                ID_NOX         = 10,  // Nitrogen Oxides
                ID_PM005       = 11,  // Particulate Matter 0.5
                ID_PM010       = 12,  // Particulate Matter 1.0
                ID_PM025       = 13,  // Particulate Matter 2.5
                ID_PM040       = 14,  // Particulate Matter 4.0
                ID_PM100       = 15,  // Particulate Matter 10.0
                ID_NOISE       = 16,  // Noise
                ID_VIBRATION   = 17,  // Vibration
                ID_STATE       = 18,  // State
                ID_BATTERY     = 19,  // Battery
                ID_SWITCH      = 20,  // On/Off, Open/Close
                ID_PRESENCE1   = 21,  // Presence1
                ID_PRESENCE2   = 22,  // Presence2
                ID_PRESENCE3   = 23,  // Presence3
                ID_DISTANCE1   = 24,  // Distance1
                ID_DISTANCE2   = 25,  // Distance2
                ID_DISTANCE3   = 26,  // Distance3
                ID_PX          = 27,  // X
                ID_PY          = 28,  // Y
                ID_PZ          = 29,  // Z
                ID_RSSI        = 30,  // RSSI
                ID_PERF1       = 31,  // Performance Metric 1
                ID_PERF2       = 32,  // Performance Metric 2
                ID_PERF3       = 33,  // Performance Metric 3
                ID_SENSORCOUNT = 34,  // The maximum number of sensor types (highest index + 1)
            };
        };  // namespace nsensorid

        struct packet_t
        {
            byte Data[256 - 4];
            s16  Size;
            s16  Capacity;

            enum
            {
                // Packet header
                HeaderSize    = 1 + 1 + 6,  // length, version, mac
                LengthOffset  = 0,
                VersionOffset = 1,
                MacOffset     = 2,
                BodyOffset    = 8,
            };

            void begin();                        // User has to fill in MAC address manually
            void begin(u8 const* mac);           // User provides MAC address
            void write(u8 const* data, u8 len);  // Write an block of data (max 255 bytes)
            void write(u16 value);               // Write a 16 bit value
            void write(u32 value);               // Write a 32 bit value
            void finalize();                     // returns the number of sensor values written
        };

        struct sensorpacket_t
        {
            byte Data[64 - 6];
            s16  Count;
            s16  Size;
            s16  Capacity;

            enum
            {
                // Packet header
                HeaderSize    = 1 + 1 + 6,  // length, version
                LengthOffset  = 0,
                VersionOffset = 1,
                MacOffset     = 2,
                DataOffset    = 8,
            };

            void begin();                                  // User has to fill in MAC address manually
            void begin(u8 const* mac);                     // User provides MAC address
            void write(nsensorid::value_t id, u16 value);  // Write a sensor value
            s32  count() const { return Count; }           // returns the number of sensor values written
            void finalize();                               // returns the number of sensor values written
        };

    }  // namespace npacket
}  // namespace ncore

#endif  // __rCORE_PACKET_H__
