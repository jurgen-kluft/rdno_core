#include "rdno_core/c_target.h"
#include "rdno_core/c_malloc.h"
#include "rdno_core/c_nvstore.h"
#include "rdno_core/c_serial.h"
#include "rdno_core/c_task.h"
#include "rdno_core/c_timer.h"
#include "rdno_core/c_app.h"

namespace ncore
{
    state_t           gState;
    nconfig::config_t gConfig;

};  // namespace ncore

#ifdef TARGET_ARDUINO

#    include "Arduino.h"

void setup()
{
    ncore::gState.config  = &ncore::gConfig;
    ncore::gState.time_ms = 0;
    ncore::gState.wifi    = nullptr;
    ncore::gState.node    = nullptr;
    ncore::gState.flags   = 0;

    ncore::nserial::begin();                     // Initialize serial communication at 115200 baud
    if (!ncore::nvstore::load(&ncore::gConfig))  // Load configuration from non-volatile storage
    {                                            // If loading fails (e.g., first run or corrupted data)
        ncore::nserial::println("config: no valid configuration found, initializing defaults.");
        ncore::napp::config_init_default(&ncore::gConfig);  // Set up default configuration values
        ncore::gState.set_config(false);
    }
    else
    {
        ncore::gState.set_config(true);
    }

    ncore::gState.time_ms = ncore::ntimer::millis();
    ncore::napp::setup(&ncore::gState);
    ncore::nserial::println("Setup done...");
}

void loop()
{
    ncore::gState.time_ms = ncore::ntimer::millis();
    ncore::napp::tick(&ncore::gState);
}

#endif
