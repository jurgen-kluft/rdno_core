#ifndef __RDNO_CORE_TIMER_H__
#define __RDNO_CORE_TIMER_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace ntimer
    {
        u64 millis();
        u64 micros();

        // Delay waits for n milliseconds before resuming execution.
        // @see: https://docs.arduino.cc/language-reference/en/functions/time/delay/
        void delay(u32 ms);
    }  // namespace ntimer
}  // namespace ncore

#endif  // __RDNO_CORE_TIMER_H__
