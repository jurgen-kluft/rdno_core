#include "rdno_core/c_str.h"
#include "rdno_core/c_deepsleep.h"

#ifdef TARGET_ESP32
#    include "Arduino.h"
#endif

namespace ncore
{
    namespace nsystem
    {
        namespace ndeepsleep
        {
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

            void snooze(u32 seconds)  // start deep sleep with a timer wakeup
            {
#ifdef TARGET_ESP32
                esp_sleep_enable_timer_wakeup((uint64_t)seconds * 1000000);
                esp_deep_sleep_start();
#else
                // For non-ESP32 platforms, we can just simulate deep sleep by halting execution
#endif
            }

            // 0 = opened, 1 = closed
            // - open means GPIO is connected to GND (LOW)
            // - closed means GPIO is connected to VCC (HIGH)
            // - last_switch_state: last read state of the switch (from RTC memory, preserved through deep sleep)
            // - stable_switch_state: stable state of the switch (read from GPIO with debounce filter)

            const s8 SWITCH_OPEN  = 0;
            const s8 SWITCH_CLOSE = 1;

            s8 as_switch_wakeup(s8 last_switch_state, s8 stable_switch_state)
            {
                nwakeup::reason_t reason = nwakeup::reason();
                if (reason == nwakeup::REASON_WAKEUP_GPIO)
                {
                    return (last_switch_state == SWITCH_OPEN) ? SWITCH_CLOSE : SWITCH_OPEN;
                }
                else
                {
                    return stable_switch_state;
                }
            }

            void as_switch_sleep(u8 pin, s8 switch_state)
            {
                if (switch_state == SWITCH_OPEN)  // LOW
                {
#ifdef TARGET_ESP32
                    esp_sleep_enable_gpio_wakeup();
                    gpio_wakeup_enable((gpio_num_t)pin, GPIO_INTR_HIGH_LEVEL);
#endif
                }
                else  // if (switch_state == SWITCH_CLOSE) // HIGH
                {
#ifdef TARGET_ESP32
                    esp_sleep_enable_gpio_wakeup();
                    gpio_wakeup_enable((gpio_num_t)pin, GPIO_INTR_LOW_LEVEL);
#endif
                }

#ifdef TARGET_ESP32
                esp_deep_sleep_start();
#endif
            }

        }  // namespace ndeepsleep
    }  // namespace nsystem
}  // namespace ncore
