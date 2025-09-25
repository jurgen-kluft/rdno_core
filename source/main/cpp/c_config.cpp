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

        // Message example: "ssid=OBNOSIS8, password=MySecretPassword, remote_server=10.0.0.22, remote_port=1234"
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
            if (config == nullptr || (id < 0 || id >= PARAM_ID_MAX_COUNT))
                return;
            switch (config->m_param_types[id])
            {
                case nconfig::PARAM_TYPE_NONE: break;
                case nconfig::PARAM_TYPE_STRING: nconfig::set_string(config, id, str); break;
                case nconfig::PARAM_TYPE_S32: nconfig::parse_int(config, id, str); break;
                case nconfig::PARAM_TYPE_BOOL: nconfig::parse_bool(config, id, str); break;
            }
        }

        void parse_int(config_t* config, s16 id, const str_t& str)
        {
            if (config == nullptr || (id < 0 || id >= PARAM_ID_MAX_COUNT))
                return;

            s32 value = 0;
            if (from_str(str, &value, 10))
            {
                set_int(config, id, value);
            }
        }

        void parse_bool(config_t* config, s16 id, const str_t& str)
        {
            if (config == nullptr || (id < 0 || id >= PARAM_ID_MAX_COUNT))
                return;
            bool boolean = false;
            if (from_str(str, &boolean))
                set_bool(config, id, boolean);
        }

        bool set_string(config_t* config, s16 id, const str_t& str)
        {
            if (config == nullptr || (id < 0 || id >= PARAM_ID_MAX_COUNT) || str_len(str) >= PARAM_ID_STRING_MAX_LENGTH)
                return false;

            s32 str_index;
            if (config->m_param_types[id] == PARAM_TYPE_NONE)
            {
                if (config->m_param_values[PARAM_ID_STRING_COUNT] >= PARAM_ID_STRING_MAX_COUNT)
                    return false;  // No more space for strings

                str_index                  = config->m_param_values[PARAM_ID_STRING_COUNT];
                config->m_param_types[id]  = PARAM_TYPE_STRING;
                config->m_param_values[id] = (str_index << 8) | str_len(str);
                config->m_param_values[PARAM_ID_STRING_COUNT] += 1;
            }
            else
            {
                str_index                  = config->m_param_values[id] >> 5;
                config->m_param_values[id] = (str_index * PARAM_ID_STRING_MAX_LENGTH) + str_len(str);
            }

            str_t dst = str_mutable(&config->m_strings[str_index * PARAM_ID_STRING_MAX_LENGTH], PARAM_ID_STRING_MAX_LENGTH);
            str_append(dst, str);
            return true;
        }

        bool get_string(const config_t* config, s16 id, str_t& outStr)
        {
            outStr = str_empty();
            if (config == nullptr || (id < 0 || id >= PARAM_ID_MAX_COUNT))
                return false;
            if (config->m_param_types[id] != PARAM_TYPE_STRING)
                return false;
            s32 const str_value  = config->m_param_values[id];
            s32 const str_index  = str_value >> 8;
            s32 const str_length = str_value & 0xFF;
            if (str_index < 0 || str_index >= config->m_param_values[PARAM_ID_STRING_COUNT])
                return false;
            outStr = str_const_n(&config->m_strings[str_index * PARAM_ID_STRING_MAX_LENGTH], str_length);
            return true;
        }

        void set_int(config_t* config, s16 id, s32 value)
        {
            if (config == nullptr || (id < 0 || id >= PARAM_ID_MAX_COUNT))
                return;
            config->m_param_types[id]  = PARAM_TYPE_S32;
            config->m_param_values[id] = value;
        }

        bool get_int(const config_t* config, s16 id, s32& outValue)
        {
            if (config == nullptr || (id < 0 || id >= PARAM_ID_MAX_COUNT) || config->m_param_types[id] != PARAM_TYPE_S32)
                return false;
            outValue = config->m_param_values[id];
            return true;
        }

        void set_bool(config_t* config, s16 id, bool value)
        {
            if (config == nullptr || (id < 0 || id >= PARAM_ID_MAX_COUNT))
                return;
            config->m_param_types[id]  = PARAM_TYPE_BOOL;
            config->m_param_values[id] = value ? 1 : 0;
        }

        bool get_bool(const config_t* config, s16 id, bool& outValue)
        {
            if (config == nullptr || (id < 0 || id >= PARAM_ID_MAX_COUNT) || config->m_param_types[id] != PARAM_TYPE_BOOL)
                return false;
            outValue = config->m_param_values[id] == 1;
            return true;
        }

    }  // namespace nconfig
}  // namespace ncore