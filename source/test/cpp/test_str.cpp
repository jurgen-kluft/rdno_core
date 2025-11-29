#include "rcore/c_target.h"
#include "ccore/c_allocator.h"
#include "rcore/c_str.h"

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

        UNITTEST_TEST(str_const_full)
        {
            str_t str1 = str_const_full("hello world", 6, 11, 11);

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
            str_t str2 = str_const_full("hello world", 6, 11, 11);

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

            str_t str4   = str_const("Hello World");
            str_t str5   = str_const("AEIOU");
            str_t found3 = str_find_one_of(str4, str5, false);
            CHECK_EQUAL(str_len(found3), 1);
            CHECK_EQUAL(str_at(found3, 0), 'e');

            str_t str6   = str_const("xyz");
            str_t found4 = str_find_one_of(str4, str6, false);
            CHECK_TRUE(str_is_empty(found4));
        }
    }

    UNITTEST_FIXTURE(from_and_to_str)
    {
        UNITTEST_TEST(from_str_bool)
        {
            str_t str1 = str_const("true");
            str_t str2 = str_const("false");
            str_t str3 = str_const("TRUE");
            str_t str4 = str_const("FALSE");
            str_t str5 = str_const("yes");

            bool boolValue;
            CHECK_TRUE(from_str(str1, &boolValue));
            CHECK_TRUE(boolValue);
            CHECK_TRUE(from_str(str2, &boolValue));
            CHECK_FALSE(boolValue);
            CHECK_TRUE(from_str(str3, &boolValue));
            CHECK_TRUE(boolValue);
            CHECK_TRUE(from_str(str4, &boolValue));
            CHECK_FALSE(boolValue);
            CHECK_FALSE(from_str(str5, &boolValue));
        }

        UNITTEST_TEST(from_str_int)
        {
            str_t str6 = str_const("123");
            str_t str7 = str_const("-456");
            str_t str8 = str_const("7F");
            str_t str9 = str_const("invalid");

            s32 intValue;
            CHECK_TRUE(from_str(str6, &intValue, 10));
            CHECK_EQUAL(intValue, 123);
            CHECK_TRUE(from_str(str7, &intValue, 10));
            CHECK_EQUAL(intValue, -456);
            CHECK_TRUE(from_str(str8, &intValue, 16));
            CHECK_EQUAL(intValue, 127);
            CHECK_FALSE(from_str(str9, &intValue, 10));
        }

        UNITTEST_TEST(from_str_uint)
        {
            str_t str6 = str_const("123");
            str_t str7 = str_const("-456");
            str_t str8 = str_const("7F");
            str_t str9 = str_const("invalid");

            u32 uintValue;
            CHECK_TRUE(from_str(str6, &uintValue, 10));
            CHECK_EQUAL(uintValue, (u32)123);
            CHECK_FALSE(from_str(str7, &uintValue, 10));  // negative number cannot be converted to unsigned
            CHECK_TRUE(from_str(str8, &uintValue, 16));
            CHECK_EQUAL(uintValue, (u32)127);
            CHECK_FALSE(from_str(str9, &uintValue, 10));
        }

        UNITTEST_TEST(from_str_float)
        {
            str_t str10 = str_const("3.14");
            str_t str11 = str_const("-0.001");
            str_t str12 = str_const("2.71828");
            str_t str13 = str_const("invalid");

            f32 floatValue;
            CHECK_TRUE(from_str(str10, &floatValue));
            CHECK_CLOSE(floatValue, 3.14f, 0.0001f);
            CHECK_TRUE(from_str(str11, &floatValue));
            CHECK_CLOSE(floatValue, -0.001f, 0.0001f);
            CHECK_TRUE(from_str(str12, &floatValue));
            CHECK_CLOSE(floatValue, 2.71828f, 0.0001f);
            CHECK_FALSE(from_str(str13, &floatValue));
        }

        UNITTEST_TEST(to_str_bool)
        {
            char  buffer[10];
            str_t str1 = str_mutable(buffer, sizeof(buffer));

            to_str(str1, true);
            CHECK_EQUAL(str_len(str1), 4);
            CHECK_EQUAL(str_at(str1, 0), 't');
            CHECK_EQUAL(str_at(str1, 1), 'r');
            CHECK_EQUAL(str_at(str1, 2), 'u');
            CHECK_EQUAL(str_at(str1, 3), 'e');

            str_clear(str1);
            to_str(str1, false);
            CHECK_EQUAL(str_len(str1), 5);
            CHECK_EQUAL(str_at(str1, 0), 'f');
            CHECK_EQUAL(str_at(str1, 1), 'a');
            CHECK_EQUAL(str_at(str1, 2), 'l');
            CHECK_EQUAL(str_at(str1, 3), 's');
            CHECK_EQUAL(str_at(str1, 4), 'e');
        }

        UNITTEST_TEST(to_str_int)
        {
            char  buffer[20];
            str_t str1 = str_mutable(buffer, sizeof(buffer));

            to_str(str1, 12345);
            CHECK_EQUAL(str_len(str1), 5);
            CHECK_EQUAL(str_at(str1, 0), '1');
            CHECK_EQUAL(str_at(str1, 1), '2');
            CHECK_EQUAL(str_at(str1, 2), '3');
            CHECK_EQUAL(str_at(str1, 3), '4');
            CHECK_EQUAL(str_at(str1, 4), '5');
            CHECK_EQUAL(str_at(str1, 5), '\0');

            str_clear(str1);
            to_str(str1, -6789);
            CHECK_EQUAL(str_len(str1), 5);
            CHECK_EQUAL(str_at(str1, 0), '-');
            CHECK_EQUAL(str_at(str1, 1), '6');
            CHECK_EQUAL(str_at(str1, 2), '7');
            CHECK_EQUAL(str_at(str1, 3), '8');
            CHECK_EQUAL(str_at(str1, 4), '9');
            CHECK_EQUAL(str_at(str1, 5), '\0');
        }

        UNITTEST_TEST(to_str_uint)
        {
            char  buffer[20];
            str_t str1 = str_mutable(buffer, sizeof(buffer));

            to_str(str1, (u32)12345);
            CHECK_EQUAL(str_len(str1), 5);
            CHECK_EQUAL(str_at(str1, 0), '1');
            CHECK_EQUAL(str_at(str1, 1), '2');
            CHECK_EQUAL(str_at(str1, 2), '3');
            CHECK_EQUAL(str_at(str1, 3), '4');
            CHECK_EQUAL(str_at(str1, 4), '5');
            CHECK_EQUAL(str_at(str1, 5), '\0');
        }

        UNITTEST_TEST(to_str_float)
        {
            char  buffer[20];
            str_t str1 = str_mutable(buffer, sizeof(buffer));

            to_str(str1, 3.14159f, 3);
            CHECK_EQUAL(str_len(str1), 5);
            CHECK_EQUAL(str_at(str1, 0), '3');
            CHECK_EQUAL(str_at(str1, 1), '.');
            CHECK_EQUAL(str_at(str1, 2), '1');
            CHECK_EQUAL(str_at(str1, 3), '4');
            CHECK_EQUAL(str_at(str1, 4), '2');  // rounded
            CHECK_EQUAL(str_at(str1, 5), '\0');

            str_clear(str1);
            to_str(str1, -0.005f, 2);
            CHECK_EQUAL(str_len(str1), 5);
            CHECK_EQUAL(str_at(str1, 0), '-');
            CHECK_EQUAL(str_at(str1, 1), '0');
            CHECK_EQUAL(str_at(str1, 2), '.');
            CHECK_EQUAL(str_at(str1, 3), '0');  // rounded
            CHECK_EQUAL(str_at(str1, 4), '1');  // rounded
            CHECK_EQUAL(str_at(str1, 5), '\0');

            str_clear(str1);
            to_str(str1, 2.0f, 0);
            CHECK_EQUAL(str_len(str1), 1);
            CHECK_EQUAL(str_at(str1, 0), '2');
            CHECK_EQUAL(str_at(str1, 1), '\0');

            str_clear(str1);
            to_str(str1, 2.0f, 6);
            CHECK_EQUAL(str_len(str1), 8);
            CHECK_EQUAL(str_at(str1, 0), '2');
            CHECK_EQUAL(str_at(str1, 1), '.');
            CHECK_EQUAL(str_at(str1, 2), '0');
            CHECK_EQUAL(str_at(str1, 3), '0');
            CHECK_EQUAL(str_at(str1, 4), '0');
            CHECK_EQUAL(str_at(str1, 5), '0');
            CHECK_EQUAL(str_at(str1, 6), '0');
            CHECK_EQUAL(str_at(str1, 7), '0');
            CHECK_EQUAL(str_at(str1, 8), '\0');
        }
    }

    UNITTEST_FIXTURE(append)
    {
        UNITTEST_TEST(str_append_str)
        {
            char  buffer[30];
            str_t str1 = str_mutable(buffer, sizeof(buffer));
            str_t str2 = str_const("hello ");
            str_t str3 = str_const("world");

            str_append(str1, str2);
            CHECK_EQUAL(str_len(str1), str_len(str2));
            CHECK_EQUAL(str_at(str1, 0), 'h');
            CHECK_EQUAL(str_at(str1, 5), ' ');

            str_append(str1, str3);
            CHECK_EQUAL(str_len(str1), str_len(str2) + str_len(str3));
            CHECK_EQUAL(str_at(str1, 6), 'w');
            CHECK_EQUAL(str_at(str1, 10), 'd');

            str_append(str1, "!!!");
            CHECK_EQUAL(str_len(str1), 14);
            CHECK_EQUAL(str_at(str1, 11), '!');
            CHECK_EQUAL(str_at(str1, 13), '!');

            str_clear(str1);
            str_t array[3] = {str_const("one "), str_const("two "), str_const("three")};
            str_append(str1, array, 3);
            CHECK_EQUAL(str_len(str1), 13);
            CHECK_EQUAL(str_at(str1, 0), 'o');
            CHECK_EQUAL(str_at(str1, 12), 'e');
        }

        UNITTEST_TEST(str_append_cstr)
        {
            char  buffer[30];
            str_t str1 = str_mutable(buffer, sizeof(buffer));

            str_append(str1, "hello ");
            CHECK_EQUAL(str_len(str1), 6);
            CHECK_EQUAL(str_at(str1, 0), 'h');
            CHECK_EQUAL(str_at(str1, 5), ' ');

            str_append(str1, "world");
            CHECK_EQUAL(str_len(str1), 11);
            CHECK_EQUAL(str_at(str1, 6), 'w');
            CHECK_EQUAL(str_at(str1, 10), 'd');

            str_append(str1, "!!!");
            CHECK_EQUAL(str_len(str1), 14);
            CHECK_EQUAL(str_at(str1, 11), '!');
            CHECK_EQUAL(str_at(str1, 13), '!');
        }

        // s16 str_append(str_t & dest, const str_t* array, s16 count);

        UNITTEST_TEST(str_append_array)
        {
            char  buffer[30];
            str_t str1     = str_mutable(buffer, sizeof(buffer));
            str_t array[3] = {str_const("one "), str_const("two "), str_const("three")};

            str_append(str1, array, 3);
            CHECK_EQUAL(str_len(str1), 13);
            CHECK_EQUAL(str_at(str1, 0), 'o');
            CHECK_EQUAL(str_at(str1, 1), 'n');
            CHECK_EQUAL(str_at(str1, 2), 'e');
            CHECK_EQUAL(str_at(str1, 4), 't');
            CHECK_EQUAL(str_at(str1, 5), 'w');
            CHECK_EQUAL(str_at(str1, 6), 'o');
            CHECK_EQUAL(str_at(str1, 8), 't');
            CHECK_EQUAL(str_at(str1, 9), 'h');
            CHECK_EQUAL(str_at(str1, 12), 'e');
        }
    }

    UNITTEST_FIXTURE(join)
    {
        UNITTEST_TEST(str_join_two)
        {
            char  buffer[30];
            str_t str1 = str_mutable(buffer, sizeof(buffer));
            str_t str2 = str_const("hello");
            str_t str3 = str_const("world");

            str_join(str1, ' ', str2, str3);
            CHECK_EQUAL(str_len(str1), str_len(str2) + str_len(str3) + 1);
            CHECK_EQUAL(str_at(str1, 0), 'h');
            CHECK_EQUAL(str_at(str1, 5), ' ');
            CHECK_EQUAL(str_at(str1, 6), 'w');
            CHECK_EQUAL(str_at(str1, 10), 'd');
        }

        // s16 str_join(str_t & dest, char sep, str_t& src1, str_t& src2, str_t& src3);
        // s16 str_join(str_t & dest, char sep, const str_t* src_array, s16 array_count);
        // s16 str_join(str_t & dest, str_t & sep, const str_t* src_array, s16 array_count);
    }
}
UNITTEST_SUITE_END
