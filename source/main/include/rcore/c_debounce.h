#ifndef __rCORE_DEBOUNCE_H__
#define __rCORE_DEBOUNCE_H__
#include "rcore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_debug.h"

namespace ncore
{
    namespace ndebounce
    {
        constexpr u8 DVALUE_UNDEFINED = 255;
        constexpr u8 DVALUE_LOW  = 0;
        constexpr u8 DVALUE_HIGH = 1;

        struct value_t
        {
            u8  m_current_state;
            u8  m_previous_poll;
            u32 m_read_starttime_ms;
        };

        struct filter_t
        {
            filter_t();
            void init(value_t& value);
            bool update(value_t& value, u8 polled_value);
            u16  m_low_to_high_debounce_interval_ms;
            u16  m_high_to_low_debounce_interval_ms;
        };
    }  // namespace ndebounce
}  // namespace ncore

#endif  // __rCORE_DEBOUNCE_H__