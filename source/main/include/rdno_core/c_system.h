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
        void get_unique_id(str_t& str); // get a unique ID for this device (e.g. chip ID or MAC address)

        void start_deepsleep();            // start deep sleep (external wakeup only)
        void start_deepsleep(u32 seconds); // start deep sleep with a timer wakeup

    } // namespace nsystem
} // namespace ncore

#endif // __RDNO_CORE_SYSTEM_H__