#include "rdno_core/c_target.h"
#include "rdno_core/c_allocator.h"
#include "rdno_core/c_packet.h"

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
            packet_t packet;
            packet.begin(1, true);

            // Write sensor values
            packet.write_value(ntype::Temperature, 25);
            packet.write_value(ntype::Humidity, 60);

            // Finalize the packet
            CHECK_EQUAL(2, packet.finalize());

            // Check the size of the packet
            CHECK_EQUAL((packet_t::HeaderSize + 2 + 2), packet.Size);
        }
    }
}
UNITTEST_SUITE_END
