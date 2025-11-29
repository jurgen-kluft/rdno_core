#include "rcore/c_packet.h"

namespace ncore
{
    namespace npacket
    {
        const u8 c_packet_version = 1;

        void packet_t::begin()
        {
            Size         = 0;                 //
            Capacity     = sizeof(Data);      // Maximum size of the packet
            Data[Size++] = 0;                 // Placeholder for length (will be set in finalize)
            Data[Size++] = c_packet_version;  // Packet version
            Size         = HeaderSize;        // Set size to header size, user has to fill in MAC address manually
        }

        void packet_t::begin(u8 const* mac)
        {
            begin();
            for (s32 i = 0; i < 6; i++)
                Data[MacOffset + i] = mac[i];  // MAC address
        }

        void packet_t::write(u8 const* data, u8 len)
        {
            Data[Size++] = len;
            for (s32 i = 0; i < len; i++)
                Data[Size++] = data[i];  // Write the sensor data
        }

        void packet_t::write(u16 value)
        {
            Data[Size++] = sizeof(u16);
            Data[Size++] = value >> 8;
            Data[Size++] = value & 0xFF;
        }

        void packet_t::write(u32 value)
        {
            Data[Size++] = sizeof(u32);
            Data[Size++] = (value >> 24) & 0xFF;
            Data[Size++] = (value >> 16) & 0xFF;
            Data[Size++] = (value >> 8) & 0xFF;
            Data[Size++] = value & 0xFF;
        }

        void packet_t::finalize()
        {
            if ((Size & 0x1) == 1)
                Data[Size++] = 0;                     // Padding to align packet size to 2 bytes
            Data[LengthOffset] = (Size >> 1) & 0xFF;  // Set the length (number of words) of the packet
        }

        // -----------------------------------------------------------------------------------------------
        // -----------------------------------------------------------------------------------------------

        void sensorpacket_t::begin()
        {
            Count        = 0;                 //
            Size         = 0;                 //
            Capacity     = sizeof(Data);      // Maximum size of the packet
            Data[Size++] = 0;                 // Placeholder for length (will be set in finalize)
            Data[Size++] = c_packet_version;  // Packet version
            Size         = HeaderSize;        // Set size to header size, user has to fill in MAC address manually
        }

        void sensorpacket_t::begin(const u8* mac)
        {
            begin();
            for (s32 i = 0; i < 6; i++)
                Data[MacOffset + i] = mac[i];  // MAC address
        }

        void sensorpacket_t::finalize()
        {
            if ((Size & 0x1) == 1)
                Data[Size++] = 0;                     // Padding to align packet size to 2 bytes
            Data[LengthOffset] = (Size >> 1) & 0xFF;  // Set the length (number of words) of the packet
        }

        void sensorpacket_t::write(nsensorid::value_t id, u16 value)
        {
            Count++;            // Increment the sensor value count
            Data[Size++] = id;  // Write the sensor stream id
            Data[Size++] = value >> 8;
            Data[Size++] = value & 0xFF;
        }

    }  // namespace npacket
}  // namespace ncore