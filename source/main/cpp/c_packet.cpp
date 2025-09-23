#include "rdno_core/c_packet.h"

// struct packet_t
// {
//     byte Data[128];
//     s32  Size;
//     s32  Capacity;
//     void begin(u32 time_ms, bool time_sync = true);
//     void write_value(ntype::value_t type,  s32 value);
//     s32 finalize();
// };
namespace ncore
{
    namespace npacket
    {
        const u8 c_packet_version = 1;

        void packet_t::begin(u32 time_ms, bool time_sync)
        {
            Size         = 0;                                             //
            Capacity     = sizeof(Data);                                  // Maximum size of the packet
            Data[Size++] = 0;                                             // Placeholder for length (will be set in finalize), will count sensor values
            Data[Size++] = c_packet_version;                              // Packet version
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

        s32 packet_t::finalize()
        {
            const s32 num_values = Data[LengthOffset];
            while ((Size & 0x01) != 0)  // Align size to 2 bytes
            {
                Data[Size++] = 0xFE;
            }
            Data[LengthOffset] = (Size >> 1) & 0xFF;  // Set the length (number of words) of the packet

            return num_values;
        }

        void packet_t::write_value(ntype::value_t type, u64 value)
        {
            // Write the sensor value to the packet
            nfieldtype::value_t field_type = nfieldtype::from_type(type);

            switch (field_type)
            {
                case nfieldtype::TypeU8:
                case nfieldtype::TypeS8:
                case nfieldtype::TypeU16:
                case nfieldtype::TypeS16:
                case nfieldtype::TypeU32:
                case nfieldtype::TypeS32:
                case nfieldtype::TypeU64:
                case nfieldtype::TypeS64:
                    Data[LengthOffset]++;  // Increment the sensor value count
                    Data[Size++] = type;   // Write the sensor type
                    break;
                default: return;
            }

            const s32 n  = (field_type & 0x7F) >> 3;  // Number of bytes to write
            Data[Size++] = value & 0xFF;
            for (s32 i = 1; i < n; i++)
            {
                value        = value >> 8;
                Data[Size++] = value & 0xFF;
            }
        }

    }  // namespace npacket
}  // namespace ncore