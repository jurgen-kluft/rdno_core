#ifndef __RDNO_CORE_STATE_H__
#define __RDNO_CORE_STATE_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nconfig
    {
        struct config_t;
    }  // namespace nconfig

    struct state_wifi_t;
    struct state_tcp_t;
    struct state_node_t;
    struct state_app_t;

    struct state_t
    {
        nconfig::config_t* config;
        u64                time_ms;
        state_wifi_t*      wifi;
        state_tcp_t*       tcp;
        state_node_t*      node;
        state_app_t*       app;
        u32                flags;

        void set_config(bool valid) { flags = (flags & ~0x1) | (valid ? 1 : 0); }
        bool has_config() const { return (flags & 0x1) != 0; }
    };

}  // namespace ncore

#endif  // __RDNO_CORE_STATE_H__
