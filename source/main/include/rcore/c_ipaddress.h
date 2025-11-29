#ifndef __RCORE_IPADDRESS_H__
#define __RCORE_IPADDRESS_H__
#include "rcore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

class IPAddress;

namespace ncore
{
    struct str_t;

    struct IPAddress_t
    {
        void reset();

        bool is_v4() const;
        bool is_v6() const;

        bool operator==(const IPAddress_t& other) const;

        static IPAddress_t from(u32 ip);
        static IPAddress_t from(u8 a, u8 b, u8 c, u8 d);

#ifdef TARGET_ARDUINO
        static void from_arduino(IPAddress_t& dest, const IPAddress& src);
        static void to_arduino(IPAddress& dest, const IPAddress_t& src);
#endif

        bool from_string(str_t const& str);
        void to_string(str_t& str) const;

        bool is_valid() const;
    private:
        u32 flags;
        u8  bytes[20];
    };

}  // namespace ncore

#endif  // __RCORE_IPADDRESS_H__
