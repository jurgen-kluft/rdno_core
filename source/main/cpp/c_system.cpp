#include "rdno_core/c_str.h"
#include "rdno_core/c_system.h"
#include "rdno_core/c_malloc.h"
#include "ccore/c_memory.h"

#ifdef TARGET_ARDUINO
#    include "Arduino.h"
#    ifdef TARGET_ESP32
#        include "esp32-hal-psram.h"
#    endif
#else
#    include <unistd.h>
#    include <sys/mman.h>
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
#    ifdef TARGET_ESP8266
            uint64_t chipid = ESP.getChipId();
            chipid <<= 24;
            chipid |= ESP.getFlashChipId();
#    else
            // For non-ESP32 platforms, we can use a placeholder unique ID
            u64 chipid = 0x123456789ABC;  // Placeholder unique ID
#    endif
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
            return (s32)(32 * 1024 * 1024);  // Assume 32MB for non-ESP32 platforms
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
#ifdef TARGET_ARDUINO
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
                    case ESP_SLEEP_WAKEUP_UNDEFINED: return REASON_WAKEUP_UNDEFINED;
                    case ESP_SLEEP_WAKEUP_EXT0: return REASON_WAKEUP_EXT0;
                    case ESP_SLEEP_WAKEUP_EXT1: return REASON_WAKEUP_EXT1;
                    case ESP_SLEEP_WAKEUP_TIMER: return REASON_WAKEUP_TIMER;
                    case ESP_SLEEP_WAKEUP_TOUCHPAD: return REASON_WAKEUP_TOUCHPAD;
                    case ESP_SLEEP_WAKEUP_ULP: return REASON_WAKEUP_ULP;
                    case ESP_SLEEP_WAKEUP_GPIO: return REASON_WAKEUP_GPIO;
                    case ESP_SLEEP_WAKEUP_UART: return REASON_WAKEUP_UART;
                    case ESP_SLEEP_WAKEUP_WIFI: return REASON_WAKEUP_WIFI;
                    case ESP_SLEEP_WAKEUP_COCPU: return REASON_WAKEUP_COCPU;
                    case ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG: return REASON_WAKEUP_COCPU_TRAP_TRIG;
                    case ESP_SLEEP_WAKEUP_BT: return REASON_WAKEUP_BT;
                    default: break;
                }
#endif
                return REASON_WAKEUP_UNDEFINED;
            }

            const char* to_string(reason_t reason)
            {
                switch (reason)
                {
                    case REASON_WAKEUP_EXT0: return "Wakeup caused by external signal using RTC_IO";
                    case REASON_WAKEUP_EXT1: return "Wakeup caused by external signal using RTC_CNTL";
                    case REASON_WAKEUP_TIMER: return "Wakeup caused by timer";
                    case REASON_WAKEUP_TOUCHPAD: return "Wakeup caused by touchpad";
                    case REASON_WAKEUP_ULP: return "Wakeup caused by ULP program";
                    case REASON_WAKEUP_GPIO: return "Wakeup caused by GPIO (light sleep only)";
                    case REASON_WAKEUP_UART: return "Wakeup caused by UART (light sleep only)";
                    case REASON_WAKEUP_WIFI: return "Wakeup caused by WIFI (light sleep only)";
                    case REASON_WAKEUP_COCPU: return "Wakeup caused by COCPU int";
                    case REASON_WAKEUP_COCPU_TRAP_TRIG: return "Wakeup caused by COCPU crash";
                    case REASON_WAKEUP_BT: return "Wakeup caused by BT (light sleep only)";
                    case REASON_WAKEUP_UNDEFINED: break;
                    default: break;
                }
                return "Wakeup was not caused by deep sleep";
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
