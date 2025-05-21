#include "rdno_core/c_target.h"
#include "rdno_core/c_allocator.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(ccore)
{
	UNITTEST_FIXTURE(types)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(type_size_check)
		{
#ifdef TARGET_32BIT
			CHECK_TRUE(sizeof(void*) == 4);
#endif

#ifdef TARGET_64BIT
			CHECK_TRUE(sizeof(void*) == 8);
#endif

			CHECK_TRUE(sizeof(u8) == 1);
			CHECK_TRUE(sizeof(u8) == 1);
			CHECK_TRUE(sizeof(s8) == 1);
			CHECK_TRUE(sizeof(u16) == 2);
			CHECK_TRUE(sizeof(s16) == 2);
			CHECK_TRUE(sizeof(u32) == 4);
			CHECK_TRUE(sizeof(s32) == 4);
			CHECK_TRUE(sizeof(u64) == 8);
			CHECK_TRUE(sizeof(s64) == 8);
			CHECK_TRUE(sizeof(f32) == 4);
			CHECK_TRUE(sizeof(f64) == 8);
			CHECK_TRUE(sizeof(bool) == 4 || sizeof(bool) == 1);

			CHECK_TRUE(sizeof(void*) == sizeof(ptr_t));
			CHECK_TRUE(sizeof(void*) == sizeof(uint_t));

			CHECK_TRUE(sizeof(uchar8)  == 1);
			CHECK_TRUE(sizeof(uchar16) == 2);
			CHECK_TRUE(sizeof(uchar32) == 4);
		}
    }
}
UNITTEST_SUITE_END
