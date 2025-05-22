#ifndef __RDNO_CORE_ADC_IO_H__
#define __RDNO_CORE_ADC_IO_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_debug.h"

namespace ncore
{
    namespace nadc
    {
        s32 analogRead(u8 pin);

    }  // namespace nadc
}  // namespace ncore

#endif  // __RDNO_CORE_ADC_IO_H__
