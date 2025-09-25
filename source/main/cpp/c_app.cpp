#include "rdno_core/c_target.h"
#include "rdno_core/c_malloc.h"
#include "rdno_core/c_task.h"
#include "rdno_core/c_app.h"

namespace ncore
{
    // TODO
    // Here we implement setup() and loop() functions for Arduino.
    // We provide namespace napplication to the user which contains
    // one function:
    //
    //    ntask::program_t setup(ntask::executor_t* exec, ntask::state_t* state)
    //
    // The user must implement this function to initialize the application and 
    // return the main program to run.
    // 

    ntask::executor_t gAppScheduler;
    ntask::state_t    gAppState;

#ifdef TARGET_ESP32

    void setup()
    {
        // serial begin

        // load config from non-volatile storage

        // call napplication::setup to return the main program
        ntask::program_t main = napp::setup(&gAppScheduler, &gAppState);

        // initialize gAppScheduler
        // initialize gAppState
    }

    void loop()
    {
        // just call the application scheduler tick method
        gAppScheduler.tick(&gAppState);
    }

#endif
};  // namespace ncore
