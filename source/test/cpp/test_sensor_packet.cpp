#include "rdno_core/c_target.h"
#include "rdno_core/c_allocator.h"
#include "rdno_core/c_sensor_packet.h"

#include "cunittest/cunittest.h"

using namespace ncore;
using namespace ncore::nsensor;

UNITTEST_SUITE_BEGIN(sensor_packet)
{
	UNITTEST_FIXTURE(write)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(test)
		{
			SensorPacket_t packet;
			packet.begin(1, 1, DeviceLocation::Bedroom | DeviceLocation::Location1);

			// Write sensor values
			packet.write_sensor_value(SensorType::Temperature,  (s8)25);
			packet.write_sensor_value(SensorType::Humidity,  (s16)60);

			// Finalize the packet
			CHECK_EQUAL(2, packet.finalize());

			// Check the size of the packet
			CHECK_EQUAL((SensorPacket_t::HeaderSize + 2 + 3) + 3, packet.Size);
		}
    }
}
UNITTEST_SUITE_END
