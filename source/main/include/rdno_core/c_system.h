#ifndef __RDNO_CORE_SYSTEM_H__
#define __RDNO_CORE_SYSTEM_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    struct str_t;

    namespace nsystem
    {
        void get_unique_id(str_t& str);  // get a unique ID for this device (e.g. chip ID or MAC address)

        bool  init_psram();              // initialize PSRAM (if available)
        bool  has_psram();               // check if PSRAM is available
        s32   total_psram();             // get total PSRAM size in bytes
        s32   free_psram();              // get free PSRAM size in bytes
        byte* alloc_psram(u32 size);     // allocate memory from PSRAM
        void  dealloc_psram(byte* ptr);  // free memory allocated from PSRAM

        namespace nwakeup
        {
            typedef s8 reason_t;

            const reason_t REASON_UNDEFINED = 0;
            const reason_t REASON_EXT0      = 1;  // Wakeup caused by external signal using RTC_IO
            const reason_t REASON_EXT1      = 2;  // Wakeup caused by external signal using RTC_CNTL
            const reason_t REASON_TIMER     = 3;  // Wakeup caused by timer
            const reason_t REASON_TOUCHPAD  = 4;  // Wakeup caused by touchpad
            const reason_t REASON_ULP       = 5;  // Wakeup caused by ULP program
            const reason_t REASON_GPIO      = 6;  // Wakeup caused by GPIO (light sleep only)
            const reason_t REASON_POWERUP   = 7;  // Wakeup caused by power on (reset)

            reason_t reason();
        }  // namespace nwakeup

        void start_deepsleep();             // start deep sleep (external wakeup only)
        void start_deepsleep(u32 seconds);  // start deep sleep with a timer wakeup

    }  // namespace nsystem
}  // namespace ncore

#endif  // __RDNO_CORE_SYSTEM_H__