#include "rdno_core/c_config.h"
#include "rdno_core/c_memory.h"
#include "rdno_core/c_serial.h"
#include "rdno_core/c_str.h"

namespace ncore
{
    namespace nconfig
    {
        void reset(config_t* config)
        {
            g_memset(config, 0, sizeof(config_t));
            config->m_version = CONFIG_VERSION;
        }

        // Message example :
        // ssid=OBNOSIS8,pw=MySecretPassword,server=10.0.0.22,port=1234
        bool parse_keyvalue(str_t& msg, str_t& outKey, str_t& outValue)
        {
            if (str_is_empty(msg))
                return false;

            str_t key = str_trim_left(msg);
            if (str_is_empty(key))
                return false;  // No more key-value pairs
            str_t equalSign = str_find(key, '=');
            if (str_is_empty(equalSign))
                return false;  // No '=' found
            key    = str_select_before(key, equalSign);
            outKey = str_trim_right(key);

            outValue = str_select_after(msg, equalSign);
            outValue = str_trim_left(outValue);
            if (str_is_empty(outValue))
                return false;  // No value found
            str_t comma = str_find(outValue, ',');
            outValue    = str_select_until(outValue, comma);
            outValue    = str_trim_right(outValue);

            msg = str_select_after(msg, comma);
            msg = str_trim_left(msg);
            return true;
        }

        void parse_value(config_t* config, s16 id, str_t const& str)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return;
            switch (config->m_param_types[id])
            {
                case nconfig::PARAM_TYPE_NONE: break;
                case nconfig::PARAM_TYPE_STRING: nconfig::set_string(config, id, str); break;
                case nconfig::PARAM_TYPE_U8: nconfig::parse_uint8(config, id, str); break;
                case nconfig::PARAM_TYPE_U16: nconfig::parse_uint16(config, id, str); break;
                case nconfig::PARAM_TYPE_U64: nconfig::parse_uint64(config, id, str); break;
            }
        }

        void parse_uint8(config_t* config, s16 id, const str_t& str)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return;

            u32 value = 0;
            if (from_str(str, &value, 10))
            {
                set_uint8(config, id, (u8)value);
            }
        }

        void parse_uint16(config_t* config, s16 id, const str_t& str)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return;

            u32 value = 0;
            if (from_str(str, &value, 10))
            {
                set_uint16(config, id, value);
            }
        }

        void parse_uint64(config_t* config, s16 id, const str_t& str)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return;

            u64 value = 0;
            if (from_str(str, &value, 10))
            {
                set_uint64(config, id, value);
            }
        }

        bool init_param(config_t* config, s16 param_id, param_type_t param_type, s16 count_id, u8 count_max)
        {
            if (config->m_param_values_u8[count_id] >= count_max)
                return false;  
            config->m_param_types[param_id]     = param_type;
            config->m_param_value_idx[param_id] = config->m_param_values_u8[count_id];
            config->m_param_values_u8[count_id] += 1;
            return true;
        }

        bool set_string(config_t* config, s16 id, const str_t& str)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT) || str_len(str) >= SETTING_STRING_MAX_LENGTH)
                return false;

            if (config->m_param_types[id] == PARAM_TYPE_NONE)
            {
                if (init_param(config, id, PARAM_TYPE_STRING, PARAM_ID_STRING_COUNT, SETTING_STRING_MAX_COUNT) == false)
                    return false;  
            }
            if (config->m_param_types[id] != PARAM_TYPE_NONE)
                return false;
            const u8 str_index          = config->m_param_value_idx[id];
            config->m_strlen[str_index] = (u8)str_len(str);
            str_t dst                   = str_mutable(&config->m_strings[str_index * SETTING_STRING_MAX_LENGTH], SETTING_STRING_MAX_LENGTH - 1);
            dst.m_ascii[0]              = 0;
            str_append(dst, str);
            return true;
        }

        bool get_string(const config_t* config, s16 id, str_t& outStr)
        {
            outStr = str_empty();
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return false;
            if (config->m_param_types[id] != PARAM_TYPE_STRING)
                return false;
            s32 const index = config->m_param_value_idx[id];
            if (index < 0 || index >= config->m_param_values_u8[PARAM_ID_STRING_COUNT])
                return false;
            s32 const   str_length = config->m_strlen[index];
            const char* str_data   = &config->m_strings[index * SETTING_STRING_MAX_LENGTH];
            outStr                 = str_const_full(str_data, 0, str_length, str_length);
            return true;
        }

        bool set_uint8(config_t* config, s16 id, u8 value)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return false;
            if (config->m_param_types[id] == PARAM_TYPE_NONE) 
            {
                if (init_param(config, id, PARAM_TYPE_U8, PARAM_ID_U8_COUNT, SETTING_U8_MAX_COUNT) == false)
                    return false;  // No more space for u8 values
            }
            if (config->m_param_types[id] != PARAM_TYPE_U8)
                return false;
            const u8 index                   = config->m_param_value_idx[id];
            config->m_param_values_u8[index] = (u8)value;
            return true;
        }

        bool get_uint8(const config_t* config, s16 id, u8& outValue)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT) || config->m_param_types[id] != PARAM_TYPE_U8)
                return false;
            const u8 index = config->m_param_value_idx[id];
            outValue       = config->m_param_values_u8[index];
            return true;
        }

        bool set_uint16(config_t* config, s16 id, u16 value)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return false;
            if (config->m_param_types[id] == PARAM_TYPE_NONE)
            {
                if (init_param(config, id, PARAM_TYPE_U16, PARAM_ID_U16_COUNT, SETTING_U16_MAX_COUNT) == false)
                    return false;  
            }
            if (config->m_param_types[id] != PARAM_TYPE_U16)
                return false;
            const u8 index                    = config->m_param_value_idx[id];
            config->m_param_values_u16[index] = value;
            return true;
        }

        bool get_uint16(const config_t* config, s16 id, u16& outValue)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT) || config->m_param_types[id] != PARAM_TYPE_U16)
                return false;
            const u8 index = config->m_param_value_idx[id];
            outValue       = config->m_param_values_u16[index];
            return true;
        }


        bool set_uint64(config_t* config, s16 id, u64 value)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return false;
            if (config->m_param_types[id] == PARAM_TYPE_NONE)
            {
                if (init_param(config, id, PARAM_TYPE_U64, PARAM_ID_U64_COUNT, SETTING_U64_MAX_COUNT) == false)
                    return false;  
            }
            if (config->m_param_types[id] != PARAM_TYPE_U64)
                return false;
            const u8 index                    = config->m_param_value_idx[id];
            config->m_param_values_u64[index] = value;
            return true;
        }

        bool get_uint64(const config_t* config, s16 id, u64& outValue)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT) || config->m_param_types[id] != PARAM_TYPE_U64)
                return false;
            const u8 index = config->m_param_value_idx[id];
            outValue       = config->m_param_values_u64[index];
            return true;
        }
    }  // namespace nconfig
}  // namespace ncore