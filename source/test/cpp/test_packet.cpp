#include "rcore/c_target.h"
#include "ccore/c_allocator.h"
#include "rcore/c_packet.h"

#include "cunittest/cunittest.h"

using namespace ncore;
using namespace ncore::npacket;

UNITTEST_SUITE_BEGIN(packet)
{
    UNITTEST_FIXTURE(write)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(test)
        {
            sensorpacket_t packet;
            packet.begin();

            // Write sensor values
            packet.write(nsensorid::ID_TEMPERATURE, 25);
            packet.write(nsensorid::ID_HUMIDITY, 60);

            // Finalize the packet
            CHECK_EQUAL(2, packet.count());

            // Check the size of the packet
            CHECK_EQUAL((packet_t::HeaderSize + 2 + 2), packet.Size);
        }
    }
}
UNITTEST_SUITE_END
