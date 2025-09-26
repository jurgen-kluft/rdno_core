#include "rdno_core/c_target.h"
#include "rdno_core/c_system.h"
#include "rdno_core/c_task.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(task)
{
	UNITTEST_FIXTURE(unique_id)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		ntask::state_t gState;

		static ntask::result_t app_main(ntask::state_t* state)
		{
			return (&gState== state) ? ntask::RESULT_OK : ntask::RESULT_ERROR;
		}

		UNITTEST_TEST(test)
		{
			ntask::executor_t* exec = ntask::init(8, 8192);

            ntask::program_t main_program = program(exec);
            xbegin(exec, main_program);
            {
				xonce(exec, app_main);
                xrun_periodic(exec, app_main, 100);  // every 100 ms
                xreturn(exec);
            }
            xend(exec);

			ntask::boot(exec, main_program);
			ntask::tick(exec, &gState);
		}
    }
}
UNITTEST_SUITE_END
