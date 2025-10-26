#ifndef __APP_COMMON_SECRETS_H__
#define __APP_COMMON_SECRETS_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    inline const char* WIFI_SSID() { return "OBNOSIS8"; }                                 // WiFi SSID
    inline const char* WIFI_PASSWORD() { return "abcdefghijkl8"; }                        // WiFi password
    inline u32         SERVER_IP() { return (192 << 24) | (168 << 16) | (8 << 8) | 88; }  // Server IP address
    inline u16         SERVER_TCPPORT() { return 31330; }                                 // Server TCP port
    inline u16         SERVER_UDPPORT() { return 31339; }                                 // Server UDP port

}  // namespace ncore

#endif
