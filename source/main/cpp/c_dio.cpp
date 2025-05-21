#include "rdno_core/c_dio.h"

#ifdef TARGET_ESP32

#include "Arduino.h"

namespace ncore
{
    namespace npin
    {
        void SetPinMode(s8 pin, s8 mode)
        {
            ::pinMode(pin, mode);
        }

        s8 ReadPin(s8 pin)
        {
            return ::digitalRead(pin);
        }

        void WritePin(s8 pin, s8 value)
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

        void SetPinMode(s8 pin, s8 mode)
        {
            if (IsValid(pin))
            {
                GPIOModes[pin] = mode;
            }
        }

        s8 ReadPin(s8 pin)
        {
            if (IsValid(pin))
            {
                return GPIOValues[pin];
            }
            return 0;
        }

        void WritePin(s8 pin, s8 value)
        {
            if (IsValid(pin))
            {
                GPIOValues[pin] = value;
            }
        }
    }  // namespace npin
}  // namespace ncore

#endif
