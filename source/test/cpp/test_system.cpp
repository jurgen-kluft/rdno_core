#include "rdno_core/c_target.h"
#include "rdno_core/c_str.h"
#include "rdno_core/c_system.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(system)
{
	UNITTEST_FIXTURE(unique_id)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(test)
		{
			char id1buf[32];
			str_t id1 = str_mutable(id1buf, sizeof(id1buf));
			char id2buf[32];
			str_t id2 = str_mutable(id2buf, sizeof(id2buf));

			nsystem::get_unique_id(id1);
			nsystem::get_unique_id(id2);

			CHECK_EQUAL(str_len(id1), str_len(id2));
			CHECK_TRUE(str_cmp(id1, id2) == 0);
			CHECK_EQUAL("123456789ABC", id1.m_const);
		}
    }
}
UNITTEST_SUITE_END
