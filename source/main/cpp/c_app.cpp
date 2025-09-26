#include "rdno_core/c_target.h"
#include "rdno_core/c_malloc.h"
#include "rdno_core/c_nvstore.h"
#include "rdno_core/c_serial.h"
#include "rdno_core/c_task.h"
#include "rdno_core/c_timer.h"
#include "rdno_core/c_app.h"

namespace ncore
{
    ntask::executor_t* gExec;
    ntask::state_t     gState;
    nconfig::config_t  gConfig;

};  // namespace ncore

#ifdef TARGET_ESP32

void setup()
{
    ncore::nserial::begin();                                // Initialize serial communication at 115200 baud
    //if (!ncore::nvstore::load(&ncore::gConfig))             // Load configuration from non-volatile storage
    {                                                       // If loading fails (e.g., first run or corrupted data)
        ncore::nserial::println("No valid configuration found, initializing defaults.");
        ncore::napp::config_init_default(&ncore::gConfig);  // Set up default configuration values
        ncore::nvstore::save(&ncore::gConfig);              // Save the default configuration to non-volatile storage
    }

    ncore::gExec = ncore::ntask::init(5, 512);

    ncore::gState.config  = &ncore::gConfig;
    ncore::gState.time_ms = ncore::ntimer::millis();
    ncore::gState.app     = nullptr;
    ncore::napp::setup(ncore::gExec, &ncore::gState);

    ncore::ntask::print_info(ncore::gExec);

    ncore::nserial::println("Setup done...");
}

void loop()
{
    ncore::gState.time_ms = ncore::ntimer::millis();
    ncore::ntask::tick(ncore::gExec, &ncore::gState);
}

#endif
