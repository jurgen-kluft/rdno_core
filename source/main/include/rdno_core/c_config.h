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
            PARAM_TYPE_U8     = 15,
            PARAM_TYPE_S8     = 16,
            PARAM_TYPE_U16    = 25,
            PARAM_TYPE_S16    = 26,
            PARAM_TYPE_U64    = 45,
            PARAM_TYPE_STRING = 50,
        };

        typedef s16 (*ParamNameToId)(const char* str, s32 len);

        enum param_id_t
        {
            // WiFi parameters
            PARAM_ID_STRING_COUNT = 0,  // u8
            PARAM_ID_U8_COUNT     = 1,  // u8
            PARAM_ID_U16_COUNT    = 2,  // u8
            PARAM_ID_U32_COUNT    = 4,  // u8
            PARAM_ID_U64_COUNT    = 5,  // u8

            PARAM_ID_WIFI_SSID     = 6,   // string
            PARAM_ID_WIFI_PASSWORD = 7,   // string
            PARAM_ID_AP_SSID       = 8,   // string
            PARAM_ID_AP_PASSWORD   = 9,   // string
            PARAM_ID_REMOTE_SERVER = 10,  // string
            PARAM_ID_REMOTE_PORT   = 11,  // u16

            PARAM_ID_T   = 12,  // u8, Temperature
            PARAM_ID_H   = 13,  // u8, Humidity
            PARAM_ID_P   = 14,  // u16, Pressure
            PARAM_ID_LUX = 15,  // u16, Light
            PARAM_ID_CO2 = 16,  // u16, CO2
            PARAM_ID_VOC = 17,  // u16, VOC
            PARAM_ID_PM1 = 18,  // u16, PM1_0
            PARAM_ID_PM2 = 19,  // u16, PM2_5
            PARAM_ID_PMA = 20,  // u16, PM10
            PARAM_ID_N   = 21,  // u8, Noise
            PARAM_ID_UV  = 22,  // u8, UV
            PARAM_ID_CO  = 23,  // u8, CO
            PARAM_ID_V   = 24,  // u8, Vibration
            PARAM_ID_S   = 25,  // u16, State
            PARAM_ID_B   = 26,  // u8, Battery
            PARAM_ID_OC  = 27,  // u8, Open/Close
            PARAM_ID_P1  = 30,  // u8, Presence1
            PARAM_ID_P2  = 31,  // u8, Presence2
            PARAM_ID_P3  = 32,  // u8, Presence3
            PARAM_ID_D1  = 33,  // u16, Distance1
            PARAM_ID_D2  = 34,  // u16, Distance2
            PARAM_ID_D3  = 35,  // u16, Distance3

            // All values are in unit of millimeter and are signed values
            // Position of the device (for motion detection)
            PARAM_ID_POS = 36,  // u64: X,Y,Z (z=height)
            PARAM_ID_LAP = 37,  // u64: LookAtPoint X,Y,Z

            // Rectangular areas (for motion detection)
            PARAM_ID_RA1 = 38,  // u64: Left,Right,Front,Back
            PARAM_ID_RA2 = 39,  // u64: Left,Right,Front,Back
            PARAM_ID_RA3 = 40,  // u64: Left,Right,Front,Back
        };

        enum esettings
        {
            SETTING_PARAM_MAX_COUNT   = 40,
            SETTING_U8_MAX_COUNT      = 25,
            SETTING_U16_MAX_COUNT     = 2,
            SETTING_U64_MAX_COUNT     = 5,
            SETTING_STRING_MAX_COUNT  = 8,
            SETTING_STRING_MAX_LENGTH = 32,
        };

        enum
        {
            CONFIG_VERSION = 0x0003,
        };

        // Size in bytes: 2 + 40 + 2*40 + 10*32 = 442 bytes
        struct config_t
        {
            u16  m_version;
            u8   m_param_types[SETTING_PARAM_MAX_COUNT];                           // parameter type
            u8   m_param_value_idx[SETTING_PARAM_MAX_COUNT];                       // parameter value index (into the respective value array)
            u8   m_param_values_u8[SETTING_U16_MAX_COUNT];                         // u8 values
            u16  m_param_values_u16[SETTING_U16_MAX_COUNT];                        // u16 values
            u64  m_param_values_u64[SETTING_U64_MAX_COUNT];                        // u64 values
            u8   m_strlen[SETTING_STRING_MAX_COUNT];                              // string length values
            char m_strings[SETTING_STRING_MAX_COUNT * SETTING_STRING_MAX_LENGTH];  // string values
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
