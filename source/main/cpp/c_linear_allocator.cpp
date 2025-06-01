#include "rdno_core/c_target.h"

#include "rdno_core/c_allocator.h"
#include "rdno_core/c_linear_allocator.h"

namespace ncore
{
    linear_alloc_t::linear_alloc_t()
        : mBuffer(nullptr)
        , mCapacity(0)
        , mUsed(0)
    {
    }

    linear_alloc_t::~linear_alloc_t()
    {
        // No need to free the buffer, it is managed externally
        mBuffer   = nullptr;
        mCapacity = 0;
        mUsed     = 0;
    }

    void linear_alloc_t::setup(void* pBuffer, uint_t capacity)
    {
        mBuffer   = pBuffer;
        mCapacity = capacity;
        mUsed     = 0;
    }

    void* linear_alloc_t::v_allocate(u32 size, u32 align)
    {
        // Align the size to the specified alignment
        size = (size + (align - 1)) & ~(align - 1);

        // Check if there is enough space
        if (mUsed + size > mCapacity)
        {
            return nullptr;  // Not enough space
        }

        // Allocate memory
        void* p = static_cast<u8*>(mBuffer) + mUsed;
        mUsed += size;
        return p;
    }

    void linear_alloc_t::v_deallocate(void* p)
    {
        // No deallocation in a linear allocator
        (void)p;
    }

};  // namespace ncore
