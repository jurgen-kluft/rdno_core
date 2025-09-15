#include "rdno_core/c_dio.h"

#ifdef TARGET_ESP32

#include "Arduino.h"

namespace ncore
{
    namespace npin
    {
        void set_pinmode(s8 pin, s8 mode)
        {
            ::pinMode(pin, mode);
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

        void set_wakeup_pin(s8 pin, s8 mode)
        {
            ::esp_sleep_enable_ext0_wakeup((gpio_num_t)pin, mode);
        }

    }  // namespace npin
}  // namespace ncore

#else

namespace ncore
{
    namespace npin
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
        
    }  // namespace npin
}  // namespace ncore

#endif
