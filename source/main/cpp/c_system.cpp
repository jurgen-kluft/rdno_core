#include "rdno_core/c_str.h"
#include "rdno_core/c_memory.h"

#ifdef TARGET_ESP32
#    include "Arduino.h"
#    include "esp_sleep.h"
#endif

namespace ncore
{
    namespace nsystem
    {
        // get a unique ID for this device (e.g. chip ID or MAC address)
        void get_unique_id(str_t& str)
        {
#ifdef TARGET_ESP32
            // For ESP32, we can use the MAC address as a unique ID
            uint64_t chipid = ESP.getEfuseMac();
#else
            // For non-ESP32 platforms, we can use a placeholder unique ID
            u64 chipid = 0x123456789ABC;  // Placeholder unique ID
#endif
            to_str(str, (u32)(chipid >> 16), 16);
            to_str(str, (u32)(chipid & 0xFFFF), 16);
        }

        void start_deepsleep()            // start deep sleep (external wakeup only)
        {
#ifdef TARGET_ESP32
            esp_deep_sleep_start(); 
#else
            // For non-ESP32 platforms, we can just simulate deep sleep by halting execution
#endif
        }

        void start_deepsleep(u32 seconds) // start deep sleep with a timer wakeup
        {
#ifdef TARGET_ESP32
            esp_sleep_enable_timer_wakeup((uint64_t)seconds * 1000000);
            esp_deep_sleep_start();
#else
            // For non-ESP32 platforms, we can just simulate deep sleep by halting execution
#endif
        }

    }  // namespace nsystem
}  // namespace ncore
