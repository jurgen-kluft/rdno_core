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
        u32                m_address;
        
        inline u8&         operator[](u8 index) { return *((u8*)&m_address + index); }
        inline u8          operator[](u8 index) const { return *((u8*)&m_address + index); }
        static IPAddress_t from(u32 ip);
        static IPAddress_t from(u8 a, u8 b, u8 c, u8 d);
    };

    struct MACAddress_t
    {
        byte m_address[6];

        void set(byte a, byte b, byte c, byte d, byte e, byte f)
        {
            m_address[0] = a;
            m_address[1] = b;
            m_address[2] = c;
            m_address[3] = d;
            m_address[4] = e;
            m_address[5] = f;
        }
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
