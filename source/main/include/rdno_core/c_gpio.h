#ifndef __RDNO_CORE_GPIO_H__
#define __RDNO_CORE_GPIO_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_debounce.h"

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

        struct output_pin_t
        {
            output_pin_t(s8 pin);
            void setup();
            void set_high();
            void set_low();

        private:
            u16 mPin;
        };

        struct input_pin_t
        {
            input_pin_t(s8 pin);
            void setup();
            bool is_high();

        private:
            u16 mPin;
        };

        struct input_debounce_pin_t
        {
            input_debounce_pin_t(s8 pin, u16 debounce_low_high_ms = 25, u16 debounce_high_low_ms = 25);
            void setup();
            bool is_high();

        private:
            ndebounce::filter_t mFilter;
            u16                 mPin;
        };

        struct analog_pin_t
        {
            analog_pin_t(s8 pin);
            void setup();
            s32  read();

        private:
            u16 mPin;
        };

    }  // namespace ngpio
}  // namespace ncore

#endif  // __RDNO_CORE_GPIO_H__
