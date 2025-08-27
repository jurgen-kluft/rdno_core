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

}  // namespace ncore

#endif  // __RDNO_CORE_NETWORK_H__
