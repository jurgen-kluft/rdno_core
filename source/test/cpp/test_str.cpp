#include "rdno_core/c_target.h"
#include "rdno_core/c_allocator.h"
#include "rdno_core/c_str.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(str)
{
    UNITTEST_FIXTURE(general)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(str_const)
        {
            str_t str1 = str_const("hello");

            CHECK_EQUAL(str1.m_str, 0);
            CHECK_EQUAL(str1.m_eos, 5);
            CHECK_EQUAL(str1.m_eos, 5);
            CHECK_EQUAL(str_len(str1), 5);
            CHECK_NULL(str1.m_ascii);
            CHECK_NOT_NULL(str1.m_const);
        }

        UNITTEST_TEST(str_const_n)
        {
            str_t str1 = str_const_n("hello world", 6, 11, 11);

            CHECK_EQUAL(str1.m_str, 6);
            CHECK_EQUAL(str1.m_eos, 11);
            CHECK_EQUAL(str1.m_eos, 11);
            CHECK_EQUAL(str_len(str1), 5);
            CHECK_NULL(str1.m_ascii);
            CHECK_NOT_NULL(str1.m_const);
        }

		UNITTEST_TEST(str_len)
		{
			str_t str1 = str_const("hello");
			str_t str2 = str_const_n("hello world", 6, 11, 11);

			CHECK_EQUAL(str_len(str1), 5);
			CHECK_EQUAL(str_len(str2), 5);
		}

		UNITTEST_TEST(str_mutable)
		{
			char   buffer[20];
			str_t str1 = str_mutable(buffer, sizeof(buffer));

			CHECK_EQUAL(str1.m_str, 0);
			CHECK_EQUAL(str1.m_end, 0);
			CHECK_EQUAL(str1.m_eos, 19);
			CHECK_EQUAL(str_len(str1), 0);
			CHECK_NOT_NULL(str1.m_ascii);
			CHECK_NOT_NULL(str1.m_const);
	 	}
    }

	UNITTEST_FIXTURE(comparison)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(str_cmp)
		{
			str_t str1 = str_const("hello");
			str_t str2 = str_const("hello");
			str_t str3 = str_const("Hello");
			str_t str4 = str_const("helloo");
			str_t str5 = str_const("hell");

			CHECK_EQUAL(str_cmp(str1, str2), 0);
			CHECK_NOT_EQUAL(str_cmp(str1, str3), 0);
			CHECK_EQUAL(str_cmp(str1, str3, false), 0);
			CHECK_EQUAL(str_cmp(str1, str4), -1);
			CHECK_EQUAL(str_cmp(str1, str5), 1);
		}

		UNITTEST_TEST(str_eq)
		{
			str_t str1 = str_const("hello");
			str_t str2 = str_const("hello");
			str_t str3 = str_const("Hello");
			str_t str4 = str_const("helloo");

			CHECK_TRUE(str_eq(str1, str2));
			CHECK_FALSE(str_eq(str1, str3));
			CHECK_TRUE(str_eq(str1, str3, false));
			CHECK_FALSE(str_eq(str1, str4));
			
		}
	}
}
UNITTEST_SUITE_END
