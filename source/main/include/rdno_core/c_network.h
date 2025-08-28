#ifndef __RDNO_CORE_NETWORK_H__
#define __RDNO_CORE_NETWORK_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    struct IPAddress_t
    {
        byte m_address[4];
    };


    struct MACAddress_t
    {
        byte m_address[6];
    };

    bool FromString(const char* str, IPAddress_t& outAddr);
    bool FromString(const char* str, MACAddress_t& outAddr);

    bool IsValidSSID(const char* ssid);
    bool IsValidPassword(const char* password);
    bool IsValidIPAddress(const char* ipAddress);

}  // namespace ncore

#endif  // __RDNO_CORE_NETWORK_H__
