#ifndef __rCORE_STATE_H__
#define __rCORE_STATE_H__
#include "rcore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rcore/c_network.secret.h"

namespace ncore
{
    struct state_wifi_t;
    struct state_tcp_t;
    struct state_udp_t;
    struct state_node_t;
    struct state_app_t;

    struct state_t
    {
        enum
        {
            FLAG_PSRAM = 0x00000001,
        };

        bool has_psram() const { return (flags & FLAG_PSRAM) != 0; }

        void reset()
        {
            WiFiSSID      = nullptr;
            WiFiPassword  = nullptr;
            for (u32 i = 0; i < 6; ++i)
            {
                MACAddress[i] = 0;
            }
            ServerIP      = 0;
            ServerTcpPort = 0;
            ServerUdpPort = 0;
            time_ms       = 0;
            wifi          = nullptr;
            tcp           = nullptr;
            udp           = nullptr;
            node          = nullptr;
            flags         = 0;
        }

        const char*   WiFiSSID;
        const char*   WiFiPassword;
        u8            MACAddress[6];
        u32           ServerIP;
        u16           ServerTcpPort;
        u16           ServerUdpPort;
        u64           time_ms;
        
        state_wifi_t* wifi;
        state_tcp_t*  tcp;
        state_udp_t*  udp;
        state_node_t* node;

        u32           flags;
    };

}  // namespace ncore

#endif  // __rCORE_STATE_H__
