#include "rdno_core/c_target.h"
#include "rdno_core/c_allocator.h"
#include "rdno_core/c_network.h"

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
            CHECK_TRUE(IsValidSSID("MyWiFi", 6));
            CHECK_TRUE(IsValidSSID("Network_123", 11));
            CHECK_TRUE(IsValidSSID("A", 1));
            CHECK_TRUE(IsValidSSID("ThisIsA32CharacterLongSSID!!!!!!", 32));
        }

        UNITTEST_TEST(invalid)
        {
            CHECK_FALSE(IsValidSSID("", 0));          
            CHECK_FALSE(IsValidSSID("ThisSSIDIsWayTooLongToBeValid123", 40)); 
            CHECK_FALSE(IsValidSSID("Invalid\x01SSID", 12)); 
        }
    }

    UNITTEST_FIXTURE(password)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(valid)
        {
            CHECK_TRUE(IsValidPassword("password", 8));
            CHECK_TRUE(IsValidPassword("P@ssw0rd123!", 12));
            CHECK_TRUE(IsValidPassword("ThisIsA64CharacterLongPassword!@ThisIsA64CharacterLongPassword!@", 64));
        }

        UNITTEST_TEST(invalid)
        {
        }
    }

    UNITTEST_FIXTURE(ipaddress)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(valid)
        {
            CHECK_TRUE(IsValidIPAddress("10.0.0.8", 8));
            CHECK_TRUE(IsValidIPAddress("192.168.8.88", 12));
        }

        UNITTEST_TEST(invalid)
        {
            CHECK_FALSE(IsValidIPAddress("256.0.0.1", 9));  // 256 is out of range
            CHECK_FALSE(IsValidIPAddress("192.168.4", 9));  // only 3 parts
            CHECK_FALSE(IsValidIPAddress("010.0.0.8", 9));  // leading zero
        }
    }

}
UNITTEST_SUITE_END
