#include "rdno_core/c_gpio.h"

#ifdef TARGET_ESP32
#include "Arduino.h"

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

        s8 read_digital(s8 pin)
        {
            return ::digitalRead(pin);
        }

        s32 read_analog(s8 pin)
        {
            return ::analogRead(pin);
        }

        void write_pin(s8 pin, s8 value)
        {
            ::digitalWrite(pin, value);
        }

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
        static u8 GPIOModes[PinsMax]  = {0};
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
