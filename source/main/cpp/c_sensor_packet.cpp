#include "rdno_core/c_sensor_packet.h"

// struct SensorPacket_t
// {
//     byte Data[128];
//     s32  Size;
//     s32  Capacity;

//     void begin(u32 time_ms);
//     void write_sensor_value(SensorType::Value type,  s32 value);
//     s32 finalize();
// };
namespace ncore
{
    namespace nsensor
    {
        void SensorPacket_t::begin(u32 time_ms, bool immediate)
        {
            Size         = 0;                                             //
            Capacity     = sizeof(Data);                                  // Maximum size of the packet
            Data[Size++] = 0;                                             // Placeholder for length (will be set in finalize), will count sensor values
            Data[Size++] = time_ms & 0xFF;                                // Time sync (3 bytes)
            time_ms      = time_ms >> 8;                                  //
            Data[Size++] = time_ms & 0xFF;                                //
            time_ms      = time_ms >> 8;                                  //
            Data[Size++] = (time_ms & 0x7F) | (immediate ? 0x80 : 0x00);  // Bit 23 indicates if this packet was sent immediately upon creation
        }

        s32 SensorPacket_t::finalize()
        {
            const s32 num_sensor_values = Data[LengthOffset];
            while ((Size & 0x01) != 0)  // Align size to 2 bytes
            {
                Data[Size++] = 0xFE;
            }
            Data[LengthOffset] = (Size >> 1) & 0xFF;  // Set the length (number of words) of the packet

            return num_sensor_values;
        }

        void SensorPacket_t::write_sensor_value(SensorType::Value type, s32 value)
        {
            // Write the sensor value to the packet
            SensorFieldType::Value field_type = ToSensorFieldType(type);

            switch (field_type)
            {
                case SensorFieldType::TypeS8:
                    Data[LengthOffset]++;  // Increment the sensor value count
                    Data[Size++] = type;
                    Data[Size++] = value & 0xFF;
                    break;
                case SensorFieldType::TypeS16:
                    Data[LengthOffset]++;  // Increment the sensor value count
                    Data[Size++] = type;
                    Data[Size++] = value & 0xFF;
                    value        = value >> 8;
                    Data[Size++] = value & 0xFF;
                    break;
                case SensorFieldType::TypeS32:
                    Data[LengthOffset]++;  // Increment the sensor value count
                    Data[Size++] = type;
                    Data[Size++] = value & 0xFF;
                    value        = value >> 8;
                    Data[Size++] = value & 0xFF;
                    value        = value >> 8;
                    Data[Size++] = value & 0xFF;
                    value        = value >> 8;
                    Data[Size++] = value & 0xFF;
                    break;
                default: break;  // Ignore unknown types
            }
        }

    }  // namespace nsensor
}  // namespace ncore