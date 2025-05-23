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
			packet.begin(1, 1);
			packet.write_info(DeviceLocation::Bedroom1, DeviceLabel::BedPresence);

			// Write sensor values
			packet.write_sensor_value(SensorType::Temperature, SensorChannel::Channel0, SensorState::On, (s8)25);
			packet.write_sensor_value(SensorType::Humidity, SensorChannel::Channel1, SensorState::On, (s16)60);
			packet.write_sensor_value(SensorType::Presence, SensorChannel::Channel2, SensorState::On, 0.75f);

			// Finalize the packet
			packet.finalize();

			// Check the size of the packet
			CHECK_TRUE(packet.Size == (SensorPacket_t::HeaderSize + 3 + 4 + 6 + 2));
		}
    }
}
UNITTEST_SUITE_END
