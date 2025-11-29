#ifndef __rCORE_NETWORK_H__
#define __rCORE_NETWORK_H__
#include "rcore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rcore/c_ipaddress.h"

namespace ncore
{
    struct str_t;

    bool from_string(str_t const& str, u8 const* outMac);
    void to_str(str_t& str, const u8* mac);

    bool is_valid_SSID(str_t const&);
    bool is_valid_password(str_t const&);
    bool is_valid_IPAddress(str_t const&);
    bool is_valid_port(u16 port);

}  // namespace ncore

#endif  // __rCORE_NETWORK_H__
