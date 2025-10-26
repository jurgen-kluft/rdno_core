#ifndef __RDNO_CORE_PACKET_H__
#define __RDNO_CORE_PACKET_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_network.h"

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
                ID_UNKNOWN     = 0x00,
                ID_TEMPERATURE = 1,   // Temperature
                ID_HUMIDITY    = 2,   // Humidity
                ID_PRESSURE    = 3,   // Pressure
                ID_LIGHT       = 4,   // Light
                ID_CO2         = 5,   // CO2
                ID_VOC         = 6,   // VOC
                ID_PM1_0       = 7,   // PM1_0
                ID_PM2_5       = 8,   // PM2_5
                ID_PM10        = 9,   // PM10
                ID_NOISE       = 10,   // Noise
                ID_UV          = 11,  // UV
                ID_CO          = 12,  // CO
                ID_VIBRATION   = 13,  // Vibration
                ID_STATE       = 14,  // State
                ID_BATTERY     = 15,  // Battery
                ID_SWITCH      = 16,  // On/Off, Open/Close
                ID_PRESENCE1   = 17,  // Presence1
                ID_PRESENCE2   = 18,  // Presence2
                ID_PRESENCE3   = 19,  // Presence3
                ID_DISTANCE1   = 20,  // Distance1
                ID_DISTANCE2   = 21,  // Distance2
                ID_DISTANCE3   = 22,  // Distance3
                ID_PX          = 23,  // X
                ID_PY          = 24,  // Y
                ID_PZ          = 25,  // Z
                ID_RSSI        = 26,  // RSSI
            };
        };  // namespace nsensorid

        struct packet_t
        {
            byte Data[64];
            s32  Size;
            s32  Capacity;

            enum
            {
                // Packet header
                HeaderSize       = 1 + 1 + 6,  // length, version
                LengthOffset     = 0,
                VersionOffset    = 1,
                MacOffset        = 2,
                SensorDataOffset = 8,
            };

            void begin(MACAddress_t const& mac);
            void write_sensor(nsensorid::value_t id, u16 value);
            s32  finalize();  // returns the number of sensor values written
        };

    }  // namespace npacket
}  // namespace ncore

#endif  // __RDNO_CORE_PACKET_H__
