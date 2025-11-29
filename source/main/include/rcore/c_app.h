#ifndef __rCORE_APP_H__
#define __rCORE_APP_H__
#include "rcore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rcore/c_state.h"

namespace ncore
{
    namespace napp
    {
        void presetup(state_t* state);
        void setup(state_t* state);
        void tick(state_t* state);
    }  // namespace napp
}  // namespace ncore

#endif  // __rCORE_ALLOCATOR_H__
