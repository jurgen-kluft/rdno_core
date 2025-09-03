#ifndef __RDNO_CORE_MALLOC_H__
#define __RDNO_CORE_MALLOC_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nsystem
    {
        byte* malloc(u32 size);
        void  free(byte* ptr);
    }  // namespace nsystem
}  // namespace ncore

#endif  // __RDNO_CORE_ALLOCATOR_H__
