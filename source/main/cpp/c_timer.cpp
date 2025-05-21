#include "rdno_core/c_timer.h"

#ifdef TARGET_ESP32

#include "Arduino.h"

namespace ncore
{
    namespace ntimer
    {
        void Delay(u32 ms)
        {
            ::delay(ms);
        }
    }  // namespace ntimer
}  // namespace ncore

#else

#include <chrono>
#include <thread>

namespace ncore
{
    namespace ntimer
    {
        void Delay(u32 ms)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }
    }  // namespace ntimer
}  // namespace ncore

#endif
