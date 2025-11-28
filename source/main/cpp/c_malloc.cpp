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

        byte* malloc_psram(u32 size)
        {
#    ifdef TARGET_ESP32
            byte* mem = (byte*)heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
            return mem;
#    else
            // PSRAM not supported on this platform, fallback to normal malloc
            return malloc(size);
#    endif
        }

        void free_psram(byte* ptr)
        {
            if (ptr != nullptr)
            {
#    ifdef TARGET_ESP32
                heap_caps_free(ptr);
#    else
                // PSRAM not supported on this platform, fallback to normal free
                free(ptr);
#    endif
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