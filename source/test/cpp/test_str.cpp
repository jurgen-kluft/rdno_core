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

		UNITTEST_TEST(str_at)
		{
			str_t str1 = str_const("hello");

			CHECK_EQUAL(str_at(str1, 0), 'h');
			CHECK_EQUAL(str_at(str1, 1), 'e');
			CHECK_EQUAL(str_at(str1, 2), 'l');
			CHECK_EQUAL(str_at(str1, 3), 'l');
			CHECK_EQUAL(str_at(str1, 4), 'o');
			CHECK_EQUAL(str_at(str1, 5), '\0');
			CHECK_EQUAL(str_at(str1, 6), '\0');
		}

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
            char  buffer[20];
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

        UNITTEST_TEST(str_has_prefix)
        {
            str_t str1 = str_const("hello");
            str_t str2 = str_const("he");
            str_t str3 = str_const("He");
            str_t str4 = str_const("hello world");

            CHECK_TRUE(str_has_prefix(str1, str2));
            CHECK_FALSE(str_has_prefix(str1, str3));
            CHECK_TRUE(str_has_prefix(str1, str3, false));
            CHECK_FALSE(str_has_prefix(str1, str4));
        }

        UNITTEST_TEST(str_has_suffix)
        {
            str_t str1 = str_const("hello");
            str_t str2 = str_const("lo");
            str_t str3 = str_const("Lo");
            str_t str4 = str_const("world hello");

            CHECK_TRUE(str_has_suffix(str1, str2));
            CHECK_FALSE(str_has_suffix(str1, str3));
            CHECK_TRUE(str_has_suffix(str1, str3, false));
            CHECK_FALSE(str_has_suffix(str1, str4));
        }

        UNITTEST_TEST(str_contains)
        {
            str_t str1 = str_const("world");
            str_t str2 = str_const("llo");
            str_t str3 = str_const("WORLD");
            str_t str4 = str_const("hello world");

            CHECK_TRUE(str_contains(str4, str1));
            CHECK_TRUE(str_contains(str4, str2));
            CHECK_FALSE(str_contains(str4, str3));
            CHECK_TRUE(str_contains(str4, str3, false));
        }

        UNITTEST_TEST(str_find)
        {
            str_t str1 = str_const("hello world");
            str_t str2 = str_const("lo");
            str_t str3 = str_const("world");
            str_t str4 = str_const("test");

            str_t found1 = str_find(str1, str2);
            CHECK_EQUAL(str_len(found1), 2);
            CHECK_EQUAL(str_at(found1, 0), 'l');
            CHECK_EQUAL(str_at(found1, 1), 'o');

            str_t found2 = str_find(str1, str3);
            CHECK_EQUAL(str_len(found2), 5);
            CHECK_EQUAL(str_at(found2, 0), 'w');
            CHECK_EQUAL(str_at(found2, 4), 'd');

            str_t found3 = str_find(str1, str4);
            CHECK_TRUE(str_is_empty(found3));
        }

		UNITTEST_TEST(str_find_last)
		{
			str_t str1 = str_const("hello world, hello universe");
			str_t str2 = str_const("hello");
			str_t str3 = str_const("world");
			str_t str4 = str_const("test");

			str_t found1 = str_find_last(str1, str2);
			CHECK_EQUAL(str_len(found1), str_len(str2));
			CHECK_EQUAL(str_at(found1, 0), 'h');
			CHECK_EQUAL(str_at(found1, 4), 'o');

			str_t found2 = str_find_last(str1, str3);
			CHECK_EQUAL(str_len(found2), str_len(str3));
			CHECK_EQUAL(str_at(found2, 0), 'w');
			CHECK_EQUAL(str_at(found2, 4), 'd');

			str_t found3 = str_find_last(str1, str4);
			CHECK_TRUE(str_is_empty(found3));
		}

        // str_t       str_find_one_of(str_t& s, str_t& chars, bool case_sensitive = true);  // find first occurrence of any of the given characters

		UNITTEST_TEST(str_find_one_of)
		{
			str_t str1 = str_const("hello world");
			str_t str2 = str_const("aeiou");
			str_t str3 = str_const("xyz");

			str_t found1 = str_find_one_of(str1, str2);
			CHECK_EQUAL(str_len(found1), 1);
			CHECK_EQUAL(str_at(found1, 0), 'e');

			str_t found2 = str_find_one_of(str1, str3);
			CHECK_TRUE(str_is_empty(found2));

			str_t str4 = str_const("Hello World");
			str_t str5 = str_const("AEIOU");
			str_t found3 = str_find_one_of(str4, str5, false);
			CHECK_EQUAL(str_len(found3), 1);
			CHECK_EQUAL(str_at(found3, 0), 'e');

			str_t str6 = str_const("xyz");
			str_t found4 = str_find_one_of(str4, str6, false);
			CHECK_TRUE(str_is_empty(found4));

		}



    }
}
UNITTEST_SUITE_END
