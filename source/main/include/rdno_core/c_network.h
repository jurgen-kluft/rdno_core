#ifndef __RDNO_CORE_NETWORK_H__
#define __RDNO_CORE_NETWORK_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    struct str_t;

    struct IPAddress_t
    {
        u32  m_address;
        void from(u32 ip);
        void from(u8 a, u8 b, u8 c, u8 d);
        byte at(s32 index) const { return (m_address >> (index * 8)) & 0xFF; }
    };

    struct MACAddress_t
    {
        byte m_address[6];
    };

    bool from_string(str_t const& str, IPAddress_t& outAddr);
    bool from_string(str_t const& str, MACAddress_t& outAddr);

    void to_str(str_t& str, IPAddress_t address, const char* sep = ".");
    void to_str(str_t& str, const MACAddress_t& address, const char* sep = ":");

    bool is_valid_SSID(str_t const&);
    bool is_valid_password(str_t const&);
    bool is_valid_IPAddress(u32 ip);
    bool is_valid_IPAddress(str_t const&);
    bool is_valid_port(u16 port);

}  // namespace ncore

#endif  // __RDNO_CORE_NETWORK_H__
