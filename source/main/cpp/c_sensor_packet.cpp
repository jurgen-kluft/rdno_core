#include "rdno_core/c_sensor_packet.h"

// struct SensorPacket_t
// {
//     byte Data[128];
//     s32  Size;
//     s32  Capacity;

//     void begin();
//     void write_header(DeviceLocation::Value location);
//     void write_sensor_value(SensorType::Value type,  s8 value);
//     void write_sensor_value(SensorType::Value type,  s16 value);
//     void write_sensor_value(SensorType::Value type,  s32 value);
//     void write_sensor_value(SensorType::Value type,  u8 value);
//     void write_sensor_value(SensorType::Value type,  u16 value);
//     void write_sensor_value(SensorType::Value type,  u32 value);
//     void end(u16 sequence, u8 version);
// };
namespace ncore
{
    namespace nsensor
    {
        void SensorPacket_t::begin(u8 sequence, u8 version)
        {
            Size         = 0;
            Capacity     = sizeof(Data);  // Maximum size of the packet
            Data[Size++] = 0;             // Placeholder for length (will be set in finalize), will count sensor values
            Data[Size++] = sequence;      // Sequence number
            Data[Size++] = version;       // Version number
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

        void SensorPacket_t::write_sensor_value(SensorType::Value type, s32 value) { write_sensor_value(type, (u32)value); }
        void SensorPacket_t::write_sensor_value(SensorType::Value type, u32 value)
        {
            Data[Size++] = type;

            // Write the sensor value to the packet
            SensorFieldType::Value field_type = ToSensorFieldType(type);
            switch (field_type)
            {
                case SensorFieldType::TypeU8:
                case SensorFieldType::TypeS8:
                    Data[LengthOffset]++;  // Increment the sensor value count
                    Data[Size++] = value & 0xFF;
                    break;
                case SensorFieldType::TypeU16:
                case SensorFieldType::TypeS16:
                    Data[LengthOffset]++;  // Increment the sensor value count
                    Data[Size++] = value & 0xFF;
                    value        = value >> 8;
                    Data[Size++] = value & 0xFF;
                    break;
                case SensorFieldType::TypeU32:
                case SensorFieldType::TypeS32:
                    Data[LengthOffset]++;  // Increment the sensor value count
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