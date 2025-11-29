#include "rcore/c_wire.h"

#ifdef TARGET_ARDUINO
#    include "Arduino.h"
#    include "Wire.h"
#endif

namespace ncore
{
    namespace nwire
    {
        void begin(s32 sdaPin, s32 sclPin)
        {
#ifdef TARGET_ARDUINO
            Wire.begin(sdaPin, sclPin);
#else
            (void)sdaPin;
            (void)sclPin;
#endif
        }
    }  // namespace nwire
}  // namespace ncore
