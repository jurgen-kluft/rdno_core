#ifndef __RDNO_CORE_APP_H__
#define __RDNO_CORE_APP_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_task.h"

namespace ncore
{
    namespace napp
    {
        void config_init_default(nconfig::config_t* config);
        s16  config_key_to_index(str_t const& str);
        
        void setup(ntask::executor_t* exec, ntask::state_t* state);

    }  // namespace napp
}  // namespace ncore

#endif  // __RDNO_CORE_ALLOCATOR_H__
