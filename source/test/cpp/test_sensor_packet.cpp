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
			packet.write_info(DeviceLocation::Bedroom | DeviceLocation::Location1, DeviceLabel::Presence);

			// Write sensor values
			packet.write_sensor_value(SensorType::Temperature, SensorModel::BME280, SensorState::On, (s8)25);
			packet.write_sensor_value(SensorType::Humidity, SensorModel::BME280, SensorState::On, (s16)60);

			// Finalize the packet
			packet.finalize();

			// Check the size of the packet
			CHECK_EQUAL((SensorPacket_t::HeaderSize + 3 + 4) + 3, packet.Size);
		}
    }
}
UNITTEST_SUITE_END
