#ifndef __RDNO_CORE_DIO_H__
#define __RDNO_CORE_DIO_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_debug.h"

namespace ncore
{
    namespace npin
    {
        const s8 High       = 1;
        const s8 Low        = 0;
        const s8 ModeInput  = 0;
        const s8 ModeOutput = 1;
        const s8 PinsMax    = 48;

        enum
        {
            PinTX = 1,
            PinRX = 3,

            PinSDA = 21,
            PinSCL = 22,

            PinSS   = 5,
            PinMOSI = 23,
            PinMISO = 19,
            PinSCK  = 18,

            PinA0  = 36,
            PinA3  = 39,
            PinA4  = 32,
            PinA5  = 33,
            PinA6  = 34,
            PinA7  = 35,
            PinA10 = 4,
            PinA11 = 0,
            PinA12 = 2,
            PinA13 = 15,
            PinA14 = 13,
            PinA15 = 12,
            PinA16 = 14,
            PinA17 = 27,
            PinA18 = 25,
            PinA19 = 26,

            PinT0 = 4,
            PinT1 = 0,
            PinT2 = 2,
            PinT3 = 15,
            PinT4 = 13,
            PinT5 = 12,
            PinT6 = 14,
            PinT7 = 27,
            PinT8 = 33,
            PinT9 = 32,

            PinDAC1 = 25,
            PinDAC2 = 26,
        };

        // Is this pin valid ?
        inline bool is_valid(s8 pin) { return pin <= PinsMax && pin > 0; }

        // PinMode configures the specified pin to behave either as an input or an output.
        // @see: https://docs.arduino.cc/language-reference/en/functions/digital-io/pinMode
        void set_pinmode(s8 pin, s8 mode);

        // Read reads the value from a specified digital pin, either HIGH or LOW.
        // @see: https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalread/
        s8 read_digital(s8 pin);
        s32 read_analog(s8 pin);

        // Write sets a HIGH or a LOW value to a digital pin.
        // @see: https://docs.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/
        void digital_write(s8 pin, s8 value);

        // Wakeup pin can be used to wake up the device from deep sleep mode.
        // The pin can be configured to trigger on a HIGH or LOW signal.
        void set_wakeup_pin(s8 pin, s8 mode);

    }  // namespace npin
}  // namespace ncore

#endif  // __RDNO_CORE_DIO_H__
