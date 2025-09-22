#include "rdno_core/c_sensor_packet.h"

// struct sensorpacket_t
// {
//     byte Data[128];
//     s32  Size;
//     s32  Capacity;
//     void begin(u32 time_ms, bool time_sync = true);
//     void write_sensor_value(SensorType::Value type,  s32 value);
//     s32 finalize();
// };
namespace ncore
{
    namespace nsensor
    {
        void sensorpacket_t::begin(u32 time_ms, bool time_sync)
        {
            Size         = 0;                                             //
            Capacity     = sizeof(Data);                                  // Maximum size of the packet
            Data[Size++] = 0;                                             // Placeholder for length (will be set in finalize), will count sensor values
            Data[Size++] = SensorPacket_Version;                          // Packet version
            Data[Size++] = 0xC3;                                          // Fixed Id 0xA5C3 (little endian)
            Data[Size++] = 0xA5;                                          //
            Data[Size++] = time_ms & 0xFF;                                // Time sync (4 bytes)
            time_ms      = time_ms >> 8;                                  //
            Data[Size++] = time_ms & 0xFF;                                //
            time_ms      = time_ms >> 8;                                  //
            Data[Size++] = time_ms & 0xFF;                                //
            time_ms      = time_ms >> 8;                                  //
            Data[Size++] = (time_ms & 0x7F) | (time_sync ? 0x80 : 0x00);  // Bit 31 indicates if this packet is a time sync packet
        }

        s32 sensorpacket_t::finalize()
        {
            const s32 num_sensor_values = Data[LengthOffset];
            while ((Size & 0x01) != 0)  // Align size to 2 bytes
            {
                Data[Size++] = 0xFE;
            }
            Data[LengthOffset] = (Size >> 1) & 0xFF;  // Set the length (number of words) of the packet

            return num_sensor_values;
        }

        void sensorpacket_t::write_sensor_value(SensorType::Value type, u64 value)
        {
            // Write the sensor value to the packet
            SensorFieldType::Value field_type = ToSensorFieldType(type);

            switch (field_type)
            {
                case SensorFieldType::TypeU8:
                case SensorFieldType::TypeS8:
                case SensorFieldType::TypeU16:
                case SensorFieldType::TypeS16:
                case SensorFieldType::TypeU32:
                case SensorFieldType::TypeS32:
                case SensorFieldType::TypeU64:
                case SensorFieldType::TypeS64:
                    Data[LengthOffset]++;  // Increment the sensor value count
                    Data[Size++] = type;   // Write the sensor type
                    break;
                default: return;
            }

            const s32 n = (field_type & 0x7F) >> 3;  // Number of bytes to write
            Data[Size++] = value & 0xFF;
            for (s32 i = 1; i < n; i++)
            {
                value        = value >> 8;
                Data[Size++] = value & 0xFF;
            }
        }

    }  // namespace nsensor
}  // namespace ncore