#ifndef __RDNO_CORE_PREFERENCES_H__
#define __RDNO_CORE_PREFERENCES_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_debug.h"

namespace ncore
{
    namespace nvstore
    {
        struct param_t
        {
            s16 m_id;    // ID of the parameter
            s16 m_type;  // u8/u16/u32/s8/s16/s32/f32/bool
            union
            {
                u8   m_u8;
                u16  m_u16;
                u32  m_u32;
                s8   m_s8;
                s16  m_s16;
                s32  m_s32;
                f32  m_f32;
                bool m_bool;
            } m_value;  // Value of the sensor
        };

        // 1 KB configuration structure
        struct config_t
        {
            char    m_ssid[32];
            char    m_password[32];
            char    m_ap_ssid[32];
            char    m_ap_password[32];
            char    m_remote_server[28];
            u16     m_remote_port;
            u16     m_param_count;
            param_t m_params[108];
        };

        void Save(config_t* config);
        void Load(config_t* config);

    }  // namespace nvstore
}  // namespace ncore

#endif  // __RDNO_CORE_PREFERENCES_H__
