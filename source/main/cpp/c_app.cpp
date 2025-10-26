#include "rdno_core/c_target.h"
#include "rdno_core/c_malloc.h"
#include "rdno_core/c_eeprom.h"
#include "rdno_core/c_serial.h"
#include "rdno_core/c_task.h"
#include "rdno_core/c_timer.h"
#include "rdno_core/c_app.h"

namespace ncore
{
    state_t gState;
};  // namespace ncore

#ifdef TARGET_ARDUINO

#    include "Arduino.h"

void setup()
{
    ncore::gState.time_ms = 0;
    ncore::gState.wifi    = nullptr;
    ncore::gState.node    = nullptr;
    ncore::gState.flags   = 0;

    ncore::gState.WiFiSSID      = ncore::WIFI_SSID();
    ncore::gState.WiFiPassword  = ncore::WIFI_PASSWORD();
    ncore::gState.ServerIP      = ncore::SERVER_IP();
    ncore::gState.ServerTcpPort = ncore::SERVER_TCPPORT();
    ncore::gState.ServerUdpPort = ncore::SERVER_UDPPORT();

    ncore::nserial::begin();  // Initialize serial communication at 115200 baud

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
