#ifndef __rCORE_WIRE_H__
#define __rCORE_WIRE_H__
#include "rcore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nwire
    {
        void begin(s32 sdaPin, s32 sclPin);
    }  // namespace nwire

}  // namespace ncore

#endif  // __rCORE_WIRE_H__
