#include "rdno_core/c_target.h"

#ifdef TARGET_ARDUINO

#    include "Arduino.h"

namespace ncore
{
    namespace nsystem
    {
        byte* malloc(uint_t size)
        {
            byte* mem = (byte*)::malloc(size);
            return mem;
        }

        void free(byte* ptr)
        {
            if (ptr != nullptr)
            {
                ::free(ptr);
            }
        }

    }  // namespace nsystem
};  // namespace ncore

#else

#    include <stdlib.h>

namespace ncore
{
    namespace nsystem
    {
        byte* malloc(u32 size)
        {
            byte* mem = (byte*)::malloc(size);
            return mem;
        }

        void free(byte* ptr)
        {
            if (ptr != nullptr)
            {
                ::free(ptr);
            }
        }
    }  // namespace nsystem
};  // namespace ncore

#endif