#include "rdno_core/c_target.h"
#include "rdno_core/c_allocator.h"
#include "rdno_core/c_printf.h"
#include "rdno_core/c_va_list.h"

#include "cunittest/cunittest.h"

namespace ncore
{
    s32  count = 0;
    char output_string[256];
    void putchar(char character) 
    {
        output_string[count++] = character;
    }
}  // namespace ncore

UNITTEST_SUITE_BEGIN(custom_printf)
{
    UNITTEST_FIXTURE(general)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(test1) 
        { 
            ncore::count = 0;
            ncore::printf("Hello World!\n"); 
            CHECK_EQUAL(13, ncore::count);
        }

        UNITTEST_TEST(test2) 
        { 
            ncore::count = 0;
            ncore::printf("Hello %s!\n", ncore::va_t("World")); 
            CHECK_EQUAL(13, ncore::count);
        }
    }
}
UNITTEST_SUITE_END
