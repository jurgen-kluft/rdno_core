#include "rcore/c_target.h"
#include "rcore/c_malloc.h"
#include "rcore/c_eeprom.h"
#include "rcore/c_serial.h"
#include "rcore/c_task.h"
#include "rcore/c_timer.h"
#include "rcore/c_app.h"

#ifdef TARGET_ARDUINO

namespace ncore
{
    static state_t gState;
};  // namespace ncore

#    include "Arduino.h"

void setup()
{
    ncore::gState.reset();
    ncore::napp::presetup(&ncore::gState);

    ncore::gState.WiFiSSID      = ncore::WIFI_SSID();
    ncore::gState.WiFiPassword  = ncore::WIFI_PASSWORD();
    ncore::gState.ServerIP      = ncore::SERVER_IP();
    ncore::gState.ServerTcpPort = ncore::SERVER_TCPPORT();
    ncore::gState.ServerUdpPort = ncore::SERVER_UDPPORT();

#    ifdef TARGET_ESP8266
    ncore::nserial::begin(ncore::nbaud::Rate74880);  // Initialize serial communication at 74880 baud
#    else
    ncore::nserial::begin(ncore::nbaud::Rate115200);  // Initialize serial communication at 115200 baud
#    endif

#    ifndef TARGET_ESP8266
    if (psramInit())
    {
        ncore::gState.flags |= ncore::state_t::FLAG_PSRAM;
        const ncore::u32 psram_size       = ESP.getPsramSize();
        const ncore::u32 free_psram       = ESP.getFreePsram();
        ncore::nserial::printf("PSRAM Size: %u Kbytes with %u Kbytes free.\n", ncore::va_t(psram_size >> 10), ncore::va_t(free_psram >> 10));
    }
    else
    {
        ncore::nserial::println("PSRAM initialization failed.");
    }
#    endif

    const ncore::u32 free_memory = ESP.getFreeHeap();
    ncore::nserial::printf("Free heap memory: %u Kbytes\n", ncore::va_t(free_memory >> 10));

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
