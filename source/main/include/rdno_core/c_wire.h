#ifndef __RDNO_CORE_WIRE_H__
#define __RDNO_CORE_WIRE_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_debug.h"

namespace ncore
{
    namespace nwire
    {
        void begin(s32 sdaPin, s32 sclPin);
    }  // namespace nwire

}  // namespace ncore

#endif  // __RDNO_CORE_WIRE_H__
