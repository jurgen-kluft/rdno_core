#include "rdno_core/c_target.h"
#include "rdno_core/c_malloc.h"
#include "rdno_core/c_nvstore.h"
#include "rdno_core/c_serial.h"
#include "rdno_core/c_task.h"
#include "rdno_core/c_timer.h"
#include "rdno_core/c_app.h"

namespace ncore
{
    ntask::executor_t* gAppExec;
    ntask::state_t    gAppState;
    nconfig::config_t gAppConfig;

#ifdef TARGET_ESP32

    void setup()
    {
        nserial::begin();                            // Initialize serial communication at 115200 baud
        if (!nvstore::load(&gAppConfig))             // Load configuration from non-volatile storage
        {                                            // If loading fails (e.g., first run or corrupted data)
            napp::config_init_default(&gAppConfig);  // Set up default configuration values
            nvstore::save(&gAppConfig);              // Save the default configuration to non-volatile storage
        }

        gAppExec = ntask::init(4, 2048);

        gAppState.config  = &gAppConfig;
        gAppState.time_ms = ntimer::millis();
        gAppState.app     = nullptr;
        napp::setup(gAppExec, &gAppState);
    }

    void loop()
    {
        gAppState.time_ms = ntimer::millis();
        ntask::tick(gAppExec, &gAppState);
    }

#endif
};  // namespace ncore
