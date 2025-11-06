#ifndef __RDNO_CORE_GPIO_H__
#define __RDNO_CORE_GPIO_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace ngpio
    {
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

        // PinMode configures the specified pin to behave either as an input or an output.
        // @see: https://docs.arduino.cc/language-reference/en/functions/digital-io/pinMode
        void set_pin_as_output(s8 pin);
        void set_pin_as_input(s8 pin);

        // Read reads the value from a specified digital pin, either HIGH or LOW.
        // @see: https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalread/
        bool read_digital(s8 pin);  // true = HIGH, false = LOW
        bool read_digital_debounced(s8 pin, u16 debounce_low_high_ms = 25, u16 debounce_high_low_ms = 25);

        s32 read_analog(s8 pin);

        // Write sets a HIGH or a LOW value to a digital pin.
        // @see: https://docs.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/
        void write_digital(s8 pin, bool value);

        // Wakeup pin can be used to wake up the device from deep sleep mode.
        // The pin can be configured to trigger on a HIGH or LOW signal.
        void set_wakeup_pin(s8 pin, s8 trigger);

    }  // namespace ngpio
}  // namespace ncore

#endif  // __RDNO_CORE_GPIO_H__
