#ifndef __RDNO_CORE_LINEAR_ALLOCATOR_H__
#define __RDNO_CORE_LINEAR_ALLOCATOR_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_allocator.h"

namespace ncore
{
    // ------------------------------------------------------------------------
    // Linear allocator
    // ------------------------------------------------------------------------

    class linear_alloc_t : public alloc_t
    {
    public:
        linear_alloc_t();
        ~linear_alloc_t();

        void setup(void* pBuffer, uint_t size);

        uint_t getSize() const { return mSize; }
        uint_t getUsed() const { return mUsed; }

    private:
        virtual void* v_allocate(u32 size, u32 align);  // Allocate memory with alignment
        virtual void  v_deallocate(void* p);            // Deallocate/Free memory

        void*  mBuffer;
        uint_t mSize;
        uint_t mUsed;
    };

}  // namespace ncore

#endif  // __RDNO_CORE_ALLOCATOR_H__
