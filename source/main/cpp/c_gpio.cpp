#include "rdno_core/c_gpio.h"
#include "rdno_core/c_timer.h"
#include "rdno_core/c_debounce.h"

#ifdef TARGET_ESP32
#    include "Arduino.h"

namespace ncore
{
    namespace ngpio
    {
        void set_pinmode(s8 pin, s8 mode)
        {
            switch (mode)
            {
                case ModeInput: ::pinMode(pin, INPUT); break;
                case ModeOutput: ::pinMode(pin, OUTPUT); break;
            }
        }

        s8 read_digital(s8 pin) { return ::digitalRead(pin); }

        s8 read_digital_debounced(s8 pin, u16 debounce_low_high_ms, u16 debounce_high_low_ms)
        {
            ndebounce::filter_t filter;
            filter.m_low_to_high_debounce_interval_ms  = debounce_low_high_ms;
            filter.m_high_to_low_debounce_interval_ms  = debounce_high_low_ms;
            ndebounce::value_t   value;

            filter.init(value);
            s8 poll = read_digital(pin);
            while (filter.update(value, poll) == false)
            {
                poll = read_digital(pin);
                ntimer::delay(5);
            }
        }

        s32 read_analog(s8 pin) { return ::analogRead(pin); }

        void write_digital(s8 pin, s8 value) { ::digitalWrite(pin, value); }

        void set_wakeup_pin(s8 pin, s8 trigger)
        {
            // switch (trigger)
            // {
            //     case High : ::esp_deep_sleep_enable_gpio_wakeup((gpio_num_t)pin, ESP_GPIO_WAKEUP_GPIO_HIGH);
            //     case Low : ::esp_deep_sleep_enable_gpio_wakeup((gpio_num_t)pin, ESP_GPIO_WAKEUP_GPIO_LOW);
            // }
        }

    }  // namespace ngpio
}  // namespace ncore

#else

namespace ncore
{
    namespace ngpio
    {
        // GPIO simulation
        static u8  GPIOModes[PinsMax]  = {0};
        static s32 GPIOValues[PinsMax] = {0};

        void set_pinmode(s8 pin, s8 mode)
        {
            if (is_valid(pin))
            {
                GPIOModes[pin] = mode;
            }
        }

        s8 read_digital(s8 pin)
        {
            if (is_valid(pin))
            {
                return GPIOValues[pin];
            }
            return 0;
        }

        s32 read_analog(s8 pin)
        {
            if (is_valid(pin))
            {
                return GPIOValues[pin];
            }
            return 0;
        }

        void write_pin(s8 pin, s8 value)
        {
            if (is_valid(pin))
            {
                GPIOValues[pin] = value;
            }
        }

        void set_wakeup_pin(s8 pin, s8 mode)
        {
            // Not supported in simulation
        }

    }  // namespace ngpio
}  // namespace ncore

#endif
