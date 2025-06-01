#include "rdno_core/c_target.h"

#ifdef TARGET_ESP32

#    include "Arduino.h"

namespace ncore
{
    byte* gMalloc(uint_t size)
    {
        byte* mem = (byte*)::malloc(size);
        return mem;
    }

    void gFree(byte* ptr)
    {
        if (ptr != nullptr)
        {
            ::free(ptr);
        }
    }

};  // namespace ncore

#else

#    include <stdlib.h>

namespace ncore
{

    byte* gMalloc(u32 size)
    {
        byte* mem = (byte*)::malloc(size);
        return mem;
    }

    void gFree(byte* ptr)
    {
        if (ptr != nullptr)
        {
            ::free(ptr);
        }
    }

};  // namespace ncore

#endif