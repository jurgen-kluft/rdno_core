#include "rcore/c_target.h"
#include "ccore/c_allocator.h"
#include "ccore/c_printf.h"
#include "ccore/c_va_list.h"

#include "cunittest/cunittest.h"

namespace ncore
{
    s32  count = 0;
    char output_string[256];
    void putchar_test(char character) 
    {
        output_string[count++] = character;
    }
}  // namespace ncore

UNITTEST_SUITE_BEGIN(custom_printf)
{
    UNITTEST_FIXTURE(general)
    {
        ncore::putchar_func_t previous_putchar = nullptr;
        UNITTEST_FIXTURE_SETUP() 
        {
            previous_putchar = ncore::putchar;
            ncore::putchar = ncore::putchar_test;
        }
        UNITTEST_FIXTURE_TEARDOWN() 
        {
            ncore::putchar = previous_putchar;
        }

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
