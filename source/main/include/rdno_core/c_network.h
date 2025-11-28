#ifndef __RDNO_CORE_NETWORK_H__
#define __RDNO_CORE_NETWORK_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#ifdef TARGET_ARDUINO
class IPAddress;
#endif

namespace ncore
{
    struct str_t;

#ifdef TARGET_ARDUINO

    typedef IPAddress IPAddress_t;

#else
    enum
    {
        IPv4 = 4,
        IPv6 = 16,
    };

    struct IPAddress_t
    {
        union
        {
            u8  bytes[16];
            u32 words[4];
        } _address;
        u8 type;

        void reset();

        inline u8& operator[](u8 index) { return _address.bytes[index]; }
        inline u8  operator[](u8 index) const { return _address.bytes[index]; }

        inline bool operator==(const IPAddress_t& other) const
        {
            if (type != other.type)
                return false;
            if (type == IPv4)
            {
                return (_address.words[3] == other._address.words[3]);
            }
            else  // IPv6
            {
                return (_address.words[0] == other._address.words[0]) && (_address.words[1] == other._address.words[1]) && (_address.words[2] == other._address.words[2]) && (_address.words[3] == other._address.words[3]);
            }
        }

        bool is_v4() const { return type == IPv4; }
        bool is_v6() const { return type == IPv6; }

        static IPAddress_t from(u32 ip);
        static IPAddress_t from(u8 a, u8 b, u8 c, u8 d);
    };
#endif

    bool from_string(str_t const& str, IPAddress_t& outAddr);
    bool from_string(str_t const& str, u8 const* outMac);

    void to_str(str_t& str, const IPAddress_t& address, const char* sep = ".");
    void to_str(str_t& str, const u8* mac, const char* sep = ":");

    bool is_valid_SSID(str_t const&);
    bool is_valid_password(str_t const&);
    bool is_valid_IPAddress(IPAddress_t const& addr);
    bool is_valid_IPAddress(str_t const&);
    bool is_valid_port(u16 port);

}  // namespace ncore

#endif  // __RDNO_CORE_NETWORK_H__
