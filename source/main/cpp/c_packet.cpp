#include "rdno_core/c_packet.h"

namespace ncore
{
    namespace npacket
    {
        const u8 c_packet_version = 1;

        void packet_t::begin(MACAddress_t const& mac)
        {
            Count        = 0;                 //
            Size         = 0;                 //
            Capacity     = sizeof(Data);      // Maximum size of the packet
            Data[Size++] = 0;                 // Placeholder for length (will be set in finalize)
            Data[Size++] = c_packet_version;  // Packet version
            for (s32 i = 0; i < 6; i++)
                Data[Size++] = mac.m_address[i];  // MAC address
        }

        void packet_t::finalize()
        {
            if ((Size & 0x1) == 1)
                Data[Size++] = 0;                     // Padding to align packet size to 2 bytes
            Data[LengthOffset] = (Size >> 1) & 0xFF;  // Set the length (number of words) of the packet
        }

        void packet_t::write_sensor(nsensorid::value_t id, u16 value)
        {
            Count++;            // Increment the sensor value count
            Data[Size++] = id;  // Write the sensor stream id
            Data[Size++] = value >> 8;
            Data[Size++] = value & 0xFF;
        }

    }  // namespace npacket
}  // namespace ncore