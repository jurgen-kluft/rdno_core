#ifndef __rCORE_MALLOC_H__
#define __rCORE_MALLOC_H__
#include "rcore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nsystem
    {
        byte* malloc(u32 size);
        void  free(byte* ptr);

        byte* malloc_psram(u32 size);
        void  free_psram(byte* ptr);

        template <typename T, typename... Args>
        T* construct(Args... args)
        {
            void* mem    = malloc(sizeof(T));
            T*    object = new (mem) T(args...);
            return object;
        }

        template <typename T>
        T* construct_array(u32 count)
        {
            void* mem    = malloc(sizeof(T) * count);
            return (T*)mem;
        }
        
        template <typename T, typename... Args>
        T* placement(u32 EXTRA = 0, Args... args)
        {
            void* mem    = malloc(sizeof(T) + EXTRA);
            T*    object = new (mem) T(args...);
            return object;
        }

        template <typename T>
        void destruct(T* p)
        {
            p->~T();
            free(p);
        }
    }  // namespace nsystem
}  // namespace ncore

#endif  // __rCORE_ALLOCATOR_H__
