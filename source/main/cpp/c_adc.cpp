#include "rdno_core/c_adc.h"

#ifdef TARGET_ESP32

#    include "Arduino.h"

namespace ncore
{
    namespace nadc
    {
        s32 AnalogRead(u8 pin)
        {
            return ::analogRead(pin);
        }

    }  // namespace nadc
}  // namespace ncore

#else

namespace ncore
{
    namespace nadc
    {
        s32 AnalogRead(u8 pin)
        {
            return 42;
        }
    }  // namespace nadc
}  // namespace ncore

#endif
