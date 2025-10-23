#ifndef __RDNO_CORE_HASH_H__
#define __RDNO_CORE_HASH_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_debug.h"

namespace ncore
{
    namespace nhash
    {
        struct context_t
        {
            u32 h[5];
            union
            {
                u32 w[16];
                u8  buffer[64];
            };
            u32 size;
            u32 totalSize;
        };

        void Init(context_t* context);
        void Update(context_t* context, u8 const* data, u32 length);
        void Final(context_t* context, u8 digest[20]);

        bool Compute(u8 const* data, u32 length, u8 digest[20]);
    }  // namespace nhash
}  // namespace ncore

#endif  // __RDNO_CORE_HASH_H__