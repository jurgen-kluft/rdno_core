#ifndef __RDNO_CORE_MALLOC_H__
#define __RDNO_CORE_MALLOC_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    // System malloc
    byte* gMalloc(u32 size);
    void  gFree(byte* ptr);

}  // namespace ncore

#endif  // __RDNO_CORE_ALLOCATOR_H__
