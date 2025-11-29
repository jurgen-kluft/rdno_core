#include "rcore/c_target.h"
#include "rcore/c_system.h"
#include "rcore/c_task.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(task)
{
    UNITTEST_FIXTURE(unique_id)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        s32 app_main_ticks = 0;
        static ntask::result_t app_main(state_t * state) { app_main_ticks++; return ntask::RESULT_OK; }
        static ntask::result_t check_if(state_t * state) { return ntask::RESULT_DONE; }
        static ntask::result_t check_if_not(state_t * state) { return ntask::RESULT_OK; }

        UNITTEST_TEST(test)
        {
        }
    }
}
UNITTEST_SUITE_END
