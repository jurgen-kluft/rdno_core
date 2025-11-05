#ifndef __RDNO_CORE_STATE_H__
#define __RDNO_CORE_STATE_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_network.secret.h"

namespace ncore
{
    struct state_wifi_t;
    struct state_tcp_t;
    struct state_udp_t;
    struct state_node_t;
    struct state_app_t;

    struct state_t
    {
        const char*      WiFiSSID;
        const char*      WiFiPassword;
        u32              ServerIP;
        u16              ServerTcpPort;
        u16              ServerUdpPort;
        u64              time_ms;
        state_wifi_t*    wifi;
        state_tcp_t*     tcp;
        state_udp_t*     udp;
        state_node_t*    node;
        u32              flags;
    };

}  // namespace ncore

#endif  // __RDNO_CORE_STATE_H__
