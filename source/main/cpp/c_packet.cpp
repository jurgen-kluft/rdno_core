#include "rdno_core/c_packet.h"

// struct packet_t
// {
//     byte Data[128];
//     s32  Size;
//     s32  Capacity;
//     void begin();
//     void write_value(u8 stream_id, u16 value);
//     s32 finalize();
// };
namespace ncore
{
    namespace npacket
    {
        const u8 c_packet_version = 1;

        void packet_t::begin()
        {
            Size         = 0;                 //
            Capacity     = sizeof(Data);      // Maximum size of the packet
            Data[Size++] = 0;                 // Placeholder for length (will be set in finalize), will count sensor values
            Data[Size++] = c_packet_version;  // Packet version
        }

        s32 packet_t::finalize()
        {
            const s32 num_values = Data[LengthOffset];
            Data[LengthOffset] = (Size >> 1) & 0xFF;  // Set the length (number of words) of the packet
            return num_values;
        }

        void packet_t::write_sensor(u16 id, u16 value)
        {
            Data[LengthOffset]++;    // Increment the sensor value count
            Data[Size++] = id >> 8;  // Write the sensor stream id
            Data[Size++] = id;       // Write the sensor stream id
            Data[Size++] = value >> 8;
            Data[Size++] = value & 0xFF;
        }

    }  // namespace npacket
}  // namespace ncore