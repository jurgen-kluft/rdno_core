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

        s8 read_pin(s8 pin)
        {
            return ::digitalRead(pin);
        }

        void write_pin(s8 pin, s8 value)
        {
            ::digitalWrite(pin, value);
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
        static u8 GPIOValues[PinsMax] = {0};

        void set_pinmode(s8 pin, s8 mode)
        {
            if (is_valid(pin))
            {
                GPIOModes[pin] = mode;
            }
        }

        s8 read_pin(s8 pin)
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
    }  // namespace npin
}  // namespace ncore

#endif
