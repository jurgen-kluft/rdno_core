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
    }  // namespace nsystem
}  // namespace ncore

#endif  // __RDNO_CORE_SYSTEM_H__