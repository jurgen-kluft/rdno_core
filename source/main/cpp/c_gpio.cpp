#include "rdno_core/c_gpio.h"
#include "rdno_core/c_timer.h"
#include "rdno_core/c_debounce.h"

#ifdef TARGET_ARDUINO
#    include "Arduino.h"

namespace ncore
{
    namespace ngpio
    {
        output_pin_t::output_pin_t(s8 pin) { mPin = (pin & 0x00FF); }
        void output_pin_t::setup()
        {
            ::pinMode(mPin & 0x00FF, OUTPUT);
            mPin |= 0x8000;
        }
        void output_pin_t::set_high()
        {
            if ((mPin & 0x8000) == 0)
                setup();
            ::digitalWrite(mPin & 0x007F, HIGH);
        }
        void output_pin_t::set_low()
        {
            if ((mPin & 0x8000) == 0)
                setup();
            ::digitalWrite(mPin & 0x007F, LOW);
        }

        input_pin_t::input_pin_t(s8 pin) { mPin = (pin & 0x00FF); }
        void input_pin_t::setup()
        {
            ::pinMode(mPin & 0x00FF, INPUT);
            mPin |= 0x8000;
        }
        bool input_pin_t::is_high()
        {
            if ((mPin & 0x8000) == 0)
                setup();
            return ::digitalRead(mPin & 0x007F) == HIGH;
        }

        input_debounce_pin_t::input_debounce_pin_t(s8 pin, u16 debounce_low_high_ms, u16 debounce_high_low_ms)
        {
            mPin                                       = (pin & 0x00FF);
            mFilter.m_low_to_high_debounce_interval_ms = debounce_low_high_ms;
            mFilter.m_high_to_low_debounce_interval_ms = debounce_high_low_ms;
        }

        void input_debounce_pin_t::setup()
        {
            ::pinMode(mPin & 0x00FF, INPUT);
            mPin |= 0x8000;
        }

        bool input_debounce_pin_t::is_high()
        {
            if ((mPin & 0x8000) == 0)
                setup();

            ndebounce::value_t value;
            mFilter.init(value);
            s8 poll = ::digitalRead(mPin & 0x007F) == HIGH ? 1 : 0;
            while (mFilter.update(value, poll) == false)
            {
                poll = ::digitalRead(mPin & 0x007F) == HIGH ? 1 : 0;
                ntimer::delay(5);
            }
            return poll == 1 ? true : false;
        }

        analog_pin_t::analog_pin_t(s8 pin) { mPin = (pin & 0x00FF) | 0x8000; }
        void analog_pin_t::setup()
        {
            // Analog pins on Arduino do not need pinMode setup
        }

        s32 analog_pin_t::read() { return ::analogRead(mPin & 0x007F); }

    }  // namespace ngpio
}  // namespace ncore

#else

namespace ncore
{
    namespace ngpio
    {
        output_pin_t::output_pin_t(s8 pin) { mPin = (pin & 0x00FF); }
        void output_pin_t::setup()
        {
            if ((mPin & 0x8000) == 0)
            {
                mPin |= 0x8000;
            }
        }
        void output_pin_t::set_high() { setup(); }
        void output_pin_t::set_low() { setup(); }

        input_pin_t::input_pin_t(s8 pin) { mPin = (pin & 0x00FF); }
        void input_pin_t::setup()
        {
            if ((mPin & 0x8000) == 0)
            {
                mPin |= 0x8000;
            }
        }
        bool input_pin_t::is_high()
        {
            setup();
            return true;
        }

    }  // namespace ngpio
}  // namespace ncore

#endif
