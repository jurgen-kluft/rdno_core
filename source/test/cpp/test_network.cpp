#include "rdno_core/c_target.h"
#include "rdno_core/c_allocator.h"
#include "rdno_core/c_network.h"
#include "rdno_core/c_str.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(network)
{
    UNITTEST_FIXTURE(ssid)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(valid)
        {
            str_t str = str_const_n("MyWiFi", 6);
            CHECK_TRUE(is_valid_SSID(str));
            str = str_const_n("Network_123", 11);
            CHECK_TRUE(is_valid_SSID(str));
            str = str_const_n("A", 1);
            CHECK_TRUE(is_valid_SSID(str));
            str = str_const_n("ThisIsA32CharacterLongSSID!!!!!!", 32);
            CHECK_TRUE(is_valid_SSID(str));
        }

        UNITTEST_TEST(invalid)
        {
            str_t str = str_const_n("", 0);
            CHECK_FALSE(is_valid_SSID(str));
            str = str_const_n("ThisSSIDIsWayTooLongToBeValid123", 40);
            CHECK_FALSE(is_valid_SSID(str));
            str = str_const_n("Invalid\x01SSID", 12);
            CHECK_FALSE(is_valid_SSID(str));
        }
    }

    UNITTEST_FIXTURE(password)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(valid)
        {
            str_t str = str_const_n("password", 8);
            CHECK_TRUE(is_valid_password(str));
            str = str_const_n("P@ssw0rd123!", 12);
            CHECK_TRUE(is_valid_password(str));
            str = str_const_n("ThisIsA64CharacterLongPassword!@ThisIsA64CharacterLongPassword!@", 64);
            CHECK_TRUE(is_valid_password(str));
        }

        UNITTEST_TEST(invalid) {}
    }

    UNITTEST_FIXTURE(ipaddress)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(valid)
        {
            str_t str = str_const_n("10.0.0.8", 8);
            CHECK_TRUE(is_valid_IPAddress(str));
            str = str_const_n("192.168.8.88", 12);
            CHECK_TRUE(is_valid_IPAddress(str));
        }

        UNITTEST_TEST(invalid)
        {
            str_t str = str_const_n("256.0.0.1", 9);  // 256 is out of range
            CHECK_FALSE(is_valid_IPAddress(str));
            str = str_const_n("192.168.4", 9);  // only 3 parts
            CHECK_FALSE(is_valid_IPAddress(str));
            str = str_const_n("010.0.0.8", 9);  // leading zero
            CHECK_FALSE(is_valid_IPAddress(str));
        }
    }
}
UNITTEST_SUITE_END
