#include "rdno_core/c_str.h"
#include "rdno_core/c_memory.h"
#include "rdno_core/c_system.h"

#ifdef TARGET_ESP32
#    include "Arduino.h"
#    include "esp32-hal-psram.h"
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

        bool init_psram()
        {
#ifdef TARGET_ESP32
            return psramInit();
#else
            return true;
#endif
        }

        bool has_psram()
        {
#ifdef TARGET_ESP32
            return psramFound();
#else
            return true;
#endif
        }

        s32 total_psram()
        {
#ifdef TARGET_ESP32
            return (s32)ESP.getPsramSize();
#else
            return 32 * 1024 * 1024;  // Assume 32MB for non-ESP32 platforms
#endif
        }
        s32 free_psram()
        {
#ifdef TARGET_ESP32
            return (s32)ESP.getFreePsram();
#else
            return (s32)(32 * 1024 * 1024 - malloc_used());  // Assume 32MB for non-ESP32 platforms
#endif
        }

        byte* alloc_psram(u32 size)
        {
#ifdef TARGET_ESP32
            return (byte*)ps_malloc(size);
#else
            return (byte*)malloc(size);
#endif
        }

        void dealloc_psram(byte* ptr)
        {
#ifdef TARGET_ESP32
            free(ptr);  // ps_free does not exist, use free instead
#else
            free(ptr);
#endif
        }

        namespace nwakeup
        {
            reason_t reason()
            {
#ifdef TARGET_ESP32
                esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
                switch (cause)
                {
                    case ESP_SLEEP_WAKEUP_EXT0: return REASON_EXT0;
                    case ESP_SLEEP_WAKEUP_EXT1: return REASON_EXT1;
                    case ESP_SLEEP_WAKEUP_TIMER: return REASON_TIMER;
                    case ESP_SLEEP_WAKEUP_TOUCHPAD: return REASON_TOUCHPAD;
                    case ESP_SLEEP_WAKEUP_ULP: return REASON_ULP;
                    case ESP_SLEEP_WAKEUP_GPIO: return REASON_GPIO;
                    case ESP_SLEEP_WAKEUP_UNDEFINED: return REASON_UNDEFINED;
                    default: return REASON_UNDEFINED;
                }
#endif
                return REASON_UNDEFINED;  // For non-ESP32 platforms, we return undefined
            }
        }  // namespace nwakeup

        void start_deepsleep()  // start deep sleep (external wakeup only)
        {
#ifdef TARGET_ESP32
            esp_deep_sleep_start();
#else
            // For non-ESP32 platforms, we can just simulate deep sleep by halting execution
#endif
        }

        void start_deepsleep(u32 seconds)  // start deep sleep with a timer wakeup
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
