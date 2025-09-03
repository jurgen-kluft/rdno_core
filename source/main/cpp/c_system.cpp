#include "rdno_core/c_str.h"
#include "rdno_core/c_memory.h"

#ifdef TARGET_ESP32

#    include <Arduino.h>

namespace ncore
{
    namespace nsystem
    {
        // get a unique ID for this device (e.g. chip ID or MAC address)
        void get_unique_id(str_t& str)
        {
            // For ESP32, we can use the MAC address as a unique ID
            uint64_t chipid = ESP.getEfuseMac();
            for (s32 i = 0; i < 6; i++)
            {
                byte byteVal             = (chipid >> (8 * (5 - i))) & 0xFF;
                str.m_ascii[str.m_end++] = to_hex_char((byteVal >> 4) & 0x0F, false);
                str.m_ascii[str.m_end++] = to_hex_char(byteVal & 0x0F, false);
            }
        }
    }  // namespace nsystem
}  // namespace ncore

#else

namespace ncore
{
    namespace nsystem
    {
        // get a unique ID for this device (e.g. chip ID or MAC address)
        void get_unique_id(str_t& str)
        {
            // For non-ESP32 platforms, we can use a placeholder unique ID
            u64 chipid = 0x123456789ABC;  // Placeholder unique ID
            for (s32 i = 0; i < 6; i++)
            {
                byte byteVal             = (chipid >> (8 * (5 - i))) & 0xFF;
                str.m_ascii[str.m_end++] = to_hex_char((byteVal >> 4) & 0x0F, false);
                str.m_ascii[str.m_end++] = to_hex_char(byteVal & 0x0F, false);
            }
        }
    }  // namespace nsystem
}  // namespace ncore

#endif