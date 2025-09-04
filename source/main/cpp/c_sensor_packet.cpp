#include "rdno_core/c_sensor_packet.h"

// struct SensorPacket_t
// {
//     byte Data[128];
//     s32  Size;
//     s32  Capacity;

//     void begin();

//     void write_header(DeviceLocation::Value location, DeviceLabel::Value label);

//     void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, s8 value);
//     void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, s16 value);
//     void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, s32 value);

//     void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, u8 value);
//     void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, u16 value);
//     void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, u32 value);

//     void write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, f32 value);

//     void end(u16 sequence, u8 version);
// };
namespace ncore
{
    namespace nsensor
    {
        void SensorPacket_t::begin(u8 sequence, u8 version)
        {
            Capacity     = sizeof(Data);  // Maximum size of the packet
            Size         = 1;             // Skip length, will be set during finalize
            Data[Size++] = sequence;      // Sequence number
            Data[Size++] = version;       // Version number
        }

        void SensorPacket_t::finalize()
        {
            // Align size to 4 bytes
            while ((Size & 0x03) != 0)
            {
                Data[Size++] = 0xFE;
            }

            // Set the length (number of words) of the packet 
            Data[0] = (Size >> 2) & 0xFF;
        }

        void SensorPacket_t::write_info(DeviceLocation::Value location, DeviceLabel::Value label)
        {
            // Write the location, label and sensor value count to the packet
            Data[Size++] = location;
            Data[Size++] = label;
            Data[Size++] = 0;  // Count of sensor values, will be updated whenever a sensor value is written
        }

        void SensorPacket_t::write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, s8 value)
        {
            Data[SensorCountOffset]++;  // Increment the count of sensor values

            // Write the sensor value to the packet
            FieldType::Value field_type = FieldType::TypeS8;
            Data[Size++]                = (type << 4) | (model);
            Data[Size++]                = (state << 4) | (field_type);

            Data[Size++] = value;
        }

        void SensorPacket_t::write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, s16 value)
        {
            Data[SensorCountOffset]++;  // Increment the count of sensor values

            // Write the sensor value to the packet
            FieldType::Value field_type = FieldType::TypeS16;
            Data[Size++]                = (type << 4) | (model);
            Data[Size++]                = (state << 4) | (field_type);

            Data[Size++] = value & 0xFF;
            value        = value >> 8;
            Data[Size++] = value & 0xFF;
        }

        void SensorPacket_t::write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, s32 value)
        {
            Data[SensorCountOffset]++;  // Increment the count of sensor values

            // Write the sensor value to the packet
            FieldType::Value field_type = FieldType::TypeS32;
            Data[Size++]                = (type << 4) | (model);
            Data[Size++]                = (state << 4) | (field_type);

            Data[Size++] = value & 0xFF;
            value        = value >> 8;
            Data[Size++] = value & 0xFF;
            value        = value >> 8;
            Data[Size++] = value & 0xFF;
            value        = value >> 8;
            Data[Size++] = value & 0xFF;
        }

        void SensorPacket_t::write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, u8 value)
        {
            Data[SensorCountOffset]++;  // Increment the count of sensor values

            // Write the sensor value to the packet
            FieldType::Value field_type = FieldType::TypeU8;
            Data[Size++]                = (type << 4) | (model);
            Data[Size++]                = (state << 4) | (field_type);

            Data[Size++] = value;
        }

        void SensorPacket_t::write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, u16 value)
        {
            Data[SensorCountOffset]++;  // Increment the count of sensor values

            // Write the sensor value to the packet
            FieldType::Value field_type = FieldType::TypeU16;
            Data[Size++]                = (type << 4) | (model);
            Data[Size++]                = (state << 4) | (field_type);

            Data[Size++] = value & 0xFF;
            value        = value >> 8;
            Data[Size++] = value & 0xFF;
        }

        void SensorPacket_t::write_sensor_value(SensorType::Value type, SensorModel::Value model, SensorState::Value state, u32 value)
        {
            Data[SensorCountOffset]++;  // Increment the count of sensor values

            // Write the sensor value to the packet
            FieldType::Value field_type = FieldType::TypeU32;
            Data[Size++]                = (type << 4) | (model);
            Data[Size++]                = (state << 4) | (field_type);

            Data[Size++] = value & 0xFF;
            value        = value >> 8;
            Data[Size++] = value & 0xFF;
            value        = value >> 8;
            Data[Size++] = value & 0xFF;
            value        = value >> 8;
            Data[Size++] = value & 0xFF;
        }

    }  // namespace nsensor
}  // namespace ncore