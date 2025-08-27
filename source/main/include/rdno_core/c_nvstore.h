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
        enum param_type_t
        {
            TYPE_NONE   = 0,
            TYPE_S32    = 15,
            TYPE_BOOL   = 17,
            TYPE_STRING = 18,
        };

        typedef s16 (*ParamNameToId)(const char* str, s32 len);

        enum params_t
        {
            // WiFi parameters
            PARAM_SSID          = 0,
            PARAM_PASSWORD      = 1,
            PARAM_AP_SSID       = 2,
            PARAM_AP_PASSWORD   = 3,
            PARAM_REMOTE_SERVER = 4,
            PARAM_REMOTE_PORT   = 5,
            PARAM_HOSTNAME      = 6,

            PARAM_SENSOR_READ_INTERVAL = 10, // Interval in milli-seconds to read sensors
            PARAM_SENSOR_SEND_INTERVAL = 11, // Interval in milli-seconds to send sensor data to server
        };

        struct param_t
        {
            s32 m_type; 
            s32 m_value;
        };

        // 1.5 KB fixed size configuration structure
        struct config_t
        {
            param_t m_params[63];
            u32     m_string_count;
            char    m_strings[32 * 32]; // Maximum of 32 strings of max 32 characters each
        };

        // Message example: "ssid=OBNOSIS8, password=MySecretPassword, remote_server=10.0.0.22, remote_port=1234"
        bool ParseKeyValue(const char*& msg, const char* msgEnd, const char*& outKey, s32& outKeyLength, const char*& outValue, s32& outValueLength);
        void ParseValue(config_t* config, s16 id, const char* str, s32 len);
        void ParseS32(config_t* config, s16 id, const char* str, s32 len);
        void ParseBool(config_t* config, s16 id, const char* str, s32 len);

        void        SetString(config_t* config, s16 id, const char* str, s32 strLen);
        const char* GetString(const config_t* config, s16 id);
        void        SetS32(config_t* config, s16 id, s32 value);
        s32         GetS32(const config_t* config, s16 id, s32 defaultValue = 0);
        void        SetBool(config_t* config, s16 id, bool value);
        bool        GetBool(const config_t* config, s16 id, bool defaultValue = false);

        void Save(config_t* config);
        void Load(config_t* config);

    } // namespace nvstore
} // namespace ncore

#endif // __RDNO_CORE_PREFERENCES_H__
