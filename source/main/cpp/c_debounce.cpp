#include "rdno_core/c_timer.h"
#include "rdno_core/c_debounce.h"

namespace ncore
{
    namespace ndebounce
    {
        filter_t::filter_t()
            : m_low_to_high_debounce_interval_ms(25)
            , m_high_to_low_debounce_interval_ms(25)
        {
        }

        void filter_t::init(value_t& value)
        {
            value.m_current_state     = DVALUE_UNDEFINED;
            value.m_previous_poll     = DVALUE_UNDEFINED;
            value.m_read_starttime_ms = 0;
        }

        bool filter_t::update(value_t& value, u8 polled_value)
        {
            const u32 current_time_ms = ntimer::millis();
            if (polled_value != value.m_previous_poll)
            {
                // Value changed, reset the timer
                value.m_previous_poll     = polled_value;
                value.m_read_starttime_ms = current_time_ms;
            }
            else
            {
                // Value is stable, check if the debounce interval has passed
                const u32 debounce_interval_ms = (polled_value == DVALUE_HIGH) ? m_low_to_high_debounce_interval_ms : m_high_to_low_debounce_interval_ms;
                if ((current_time_ms - value.m_read_starttime_ms) >= debounce_interval_ms)
                {
                    // Debounce interval passed, update the current state if it has changed
                    if (value.m_current_state != polled_value)
                    {
                        value.m_current_state = polled_value;
                        return true;  // State changed
                    }
                }
            }
            return false;  // No state change
        }

    }  // namespace ndebounce
}  // namespace ncore
