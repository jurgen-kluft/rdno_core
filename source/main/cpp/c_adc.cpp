#include "rdno_core/c_adc.h"

#ifdef TARGET_ESP32
#    include "Arduino.h"
#endif

namespace ncore
{
    namespace nadc
    {
        s32 analog_read(u8 pin)
        {
#ifdef TARGET_ESP32
            return ::analogRead(pin);
#else
            return 42;  // Dummy value
#endif
        }

    }  // namespace nadc
}  // namespace ncore

