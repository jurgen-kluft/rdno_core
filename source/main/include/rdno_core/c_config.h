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
            PARAM_TYPE_U8     = (0x10) | 1,
            PARAM_TYPE_S8     = (0x20) | 1,
            PARAM_TYPE_U16    = (0x30) | 2,
            PARAM_TYPE_S16    = (0x40) | 2,
            PARAM_TYPE_U64    = (0x50) | 8,
            PARAM_TYPE_STRING = 0x60,
        };

        typedef s16 (*ParamNameToId)(const char* str, s32 len);

        enum param_id_t
        {
            PARAM_ID_WIFI_SSID = 0,  // string
            PARAM_ID_WIFI_PASSWORD,  // string
            PARAM_ID_AP_SSID,        // string
            PARAM_ID_AP_PASSWORD,    // string
            PARAM_ID_REMOTE_SERVER,  // string
            PARAM_ID_REMOTE_PORT,    // u16
            PARAM_ID_T,              // u8, Temperature
            PARAM_ID_H,              // u8, Humidity
            PARAM_ID_P,              // u16, Pressure
            PARAM_ID_LUX,            // u16, Light
            PARAM_ID_CO2,            // u16, CO2
            PARAM_ID_VOC,            // u16, VOC
            PARAM_ID_PM1,            // u16, PM1_0
            PARAM_ID_PM2,            // u16, PM2_5
            PARAM_ID_PMA,            // u16, PM10
            PARAM_ID_N,              // u8, Noise
            PARAM_ID_UV,             // u8, UV
            PARAM_ID_CO,             // u8, CO
            PARAM_ID_V,              // u8, Vibration
            PARAM_ID_S,              // u16, State
            PARAM_ID_B,              // u8, Battery
            PARAM_ID_OC,             // u8, Open/Close
            PARAM_ID_P1,             // u8, Presence1
            PARAM_ID_P2,             // u8, Presence2
            PARAM_ID_P3,             // u8, Presence3
            PARAM_ID_D1,             // u16, Distance1
            PARAM_ID_D2,             // u16, Distance2
            PARAM_ID_D3,             // u16, Distance3
            PARAM_ID_PX,             // s16: X
            PARAM_ID_PY,             // s16: Y
            PARAM_ID_PZ,             // s16: Z
            PARAM_ID_MAX_COUNT,
        };

        enum esettings
        {
            SETTING_PARAM_MAX_COUNT   = PARAM_ID_MAX_COUNT,
            SETTING_DATA_MAX_SIZE     = 128,
            SETTING_STRING_MAX_COUNT  = 6,
            SETTING_STRING_MAX_LENGTH = 24,
        };

        enum
        {
            CONFIG_VERSION = 0x0103,
        };

        // Size in bytes: 2*3 + (1+1)*36 + 128 + 6*1 + 6*24 = 356
        struct config_t
        {
            u32  m_crc;                                                            // CRC of config_t
            u16  m_version;                                                        //
            u16  m_string_count;                                                   //
            u16  m_data_offset;                                                    //
            u8   m_param_types[SETTING_PARAM_MAX_COUNT];                           // parameter type
            u8   m_param_value_offset[SETTING_PARAM_MAX_COUNT];                    // parameter value offset (into m_param_value_data)
            u8   m_param_value_data[SETTING_DATA_MAX_SIZE];                        // array value data
            u8   m_strlen[SETTING_STRING_MAX_COUNT];                               // str lengths
            char m_strings[SETTING_STRING_MAX_COUNT * SETTING_STRING_MAX_LENGTH];  // str data
        };

        void reset(config_t* config);

        // Message example:
        // ssid=OBNOSIS8,pw=MySecretPassword,server=192.168.8.88,port=31339,T=7,HPA=8,CO2=9
        bool parse_keyvalue(str_t& msg, str_t& outKey, str_t& outValue);
        void parse_value(config_t* config, s16 id, str_t const& str);
        void parse_int8(config_t* config, s16 id, str_t const& str);
        void parse_uint8(config_t* config, s16 id, str_t const& str);
        void parse_int16(config_t* config, s16 id, str_t const& str);
        void parse_uint16(config_t* config, s16 id, str_t const& str);
        void parse_uint64(config_t* config, s16 id, str_t const& str);

        bool set_string(config_t* config, s16 id, str_t const& str);
        bool get_string(const config_t* config, s16 id, str_t& outStr);

        bool set_int8(config_t* config, s16 id, s8 value);
        bool get_int8(const config_t* config, s16 id, s8& outValue);

        bool set_uint8(config_t* config, s16 id, u8 value);
        bool get_uint8(const config_t* config, s16 id, u8& outValue);

        bool set_int16(config_t* config, s16 id, s16 value);
        bool get_int16(const config_t* config, s16 id, s16& outValue);

        bool set_uint16(config_t* config, s16 id, u16 value);
        bool get_uint16(const config_t* config, s16 id, u16& outValue);

        bool set_uint64(config_t* config, s16 id, u64 value);
        bool get_uint64(const config_t* config, s16 id, u64& outValue);
    }  // namespace nconfig
}  // namespace ncore

#endif  // __RDNO_CORE_CONFIG_H__
