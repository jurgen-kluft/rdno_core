#ifndef __RDNO_CORE_NVSTORE_H__
#define __RDNO_CORE_NVSTORE_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nvstore
    {
        void reset(config_t* config);
        void save(config_t* config);
        bool load(config_t* config);
    }  // namespace nvstore
}  // namespace ncore

#endif  // __RDNO_CORE_NVSTORE_H__
