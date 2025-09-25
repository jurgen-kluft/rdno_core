#ifndef __RDNO_CORE_CONFIG_H__
#define __RDNO_CORE_CONFIG_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    struct str_t;

    namespace nconfig
    {
        enum param_type_t
        {
            PARAM_TYPE_NONE   = 0,
            PARAM_TYPE_S32    = 15,
            PARAM_TYPE_BOOL   = 17,
            PARAM_TYPE_STRING = 18,
        };

        typedef s16 (*ParamNameToId)(const char* str, s32 len);

        enum params_t
        {
            // WiFi parameters
            PARAM_ID_STRING_COUNT  = 0,
            PARAM_ID_WIFI_SSID     = 1,
            PARAM_ID_WIFI_PASSWORD = 2,
            PARAM_ID_AP_SSID       = 3,
            PARAM_ID_AP_PASSWORD   = 4,
            PARAM_ID_REMOTE_SERVER = 5,
            PARAM_ID_REMOTE_PORT   = 6,

            PARAM_ID_SENSOR_READ_INTERVAL = 10,  // Interval in milli-seconds to read sensors
            PARAM_ID_SENSOR_SEND_INTERVAL = 11,  // Interval in milli-seconds to send sensor data to server

            PARAM_ID_MAX_COUNT         = 32,
            PARAM_ID_STRING_MAX_COUNT  = 16,
            PARAM_ID_STRING_MAX_LENGTH = 32,
        };

        enum
        {
            CONFIG_VERSION = 0x00010000,
        };

        // 1.3 KB fixed size configuration structure
        struct config_t
        {
            u32  m_version;
            s8   m_param_types[PARAM_ID_MAX_COUNT];
            s32  m_param_values[PARAM_ID_MAX_COUNT];                                 // For strings: (index * 32) + length
            char m_strings[PARAM_ID_STRING_MAX_COUNT * PARAM_ID_STRING_MAX_LENGTH];  // Maximum of 32 strings of max 32 characters each
        };

        void reset(config_t* config);

        // Message example: "ssid=OBNOSIS8, password=MySecretPassword, remote_server=10.0.0.22, remote_port=1234, device_name=Bedroom.1."
        bool parse_keyvalue(str_t& msg, str_t& outKey, str_t& outValue);
        void parse_value(config_t* config, s16 id, str_t const& str);
        void parse_int(config_t* config, s16 id, str_t const& str);
        void parse_bool(config_t* config, s16 id, str_t const& str);

        bool set_string(config_t* config, s16 id, str_t const& str);
        bool get_string(const config_t* config, s16 id, str_t& outStr);
        void set_int(config_t* config, s16 id, s32 value);
        bool get_int(const config_t* config, s16 id, s32& outValue);
        void set_bool(config_t* config, s16 id, bool value);
        bool get_bool(const config_t* config, s16 id, bool& outValue);
    }  // namespace nconfig
}  // namespace ncore

#endif  // __RDNO_CORE_CONFIG_H__
