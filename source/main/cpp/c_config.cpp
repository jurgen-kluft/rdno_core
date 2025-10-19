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
                case nconfig::PARAM_TYPE_S8: nconfig::parse_int8(config, id, str); break;
                case nconfig::PARAM_TYPE_U8: nconfig::parse_uint8(config, id, str); break;
                case nconfig::PARAM_TYPE_S16: nconfig::parse_int16(config, id, str); break;
                case nconfig::PARAM_TYPE_U16: nconfig::parse_uint16(config, id, str); break;
                case nconfig::PARAM_TYPE_U64: nconfig::parse_uint64(config, id, str); break;
            }
        }

        void parse_int8(config_t* config, s16 id, const str_t& str)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return;

            s32 value = 0;
            if (from_str(str, &value, 10))
            {
                set_int8(config, id, (s8)value);
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

        void parse_int16(config_t* config, s16 id, const str_t& str)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return;

            s32 value = 0;
            if (from_str(str, &value, 10))
            {
                set_int16(config, id, (s16)value);
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

        static inline u8   read_u8(u8 const* data, u8 offset) { return data[offset]; }
        static inline void write_u8(u8* data, u8 offset, u8 value) { data[offset] = value; }

        static inline u16  read_u16(u8 const* data, u8 offset) { return data[offset] | (data[offset + 1] << 8); }
        static inline void write_u16(u8* data, u8 offset, u16 value)
        {
            data[offset]     = (u8)(value & 0xFF);
            data[offset + 1] = (u8)((value >> 8) & 0xFF);
        }

        static inline u64 read_u64(u8 const* data, u8 offset)
        {
            u64 value = 0;
            for (s32 i = 0; i < 8; i++)
            {
                value |= ((u64)data[offset + i]) << (i * 8);
            }
            return value;
        }
        static inline void write_u64(u8* data, u8 offset, u64 value)
        {
            for (s32 i = 0; i < 8; i++)
            {
                data[offset + i] = (u8)((value >> (i * 8)) & 0xFF);
            }
        }

        bool init_param(config_t* config, s16 param_id, param_type_t param_type)
        {
            config->m_param_types[param_id] = param_type;
            if (param_type == PARAM_TYPE_STRING)
            {
                config->m_param_value_offset[param_id] = config->m_string_count;
                config->m_string_count += 1;
            }
            else
            {
                config->m_param_value_offset[param_id] = config->m_data_offset;
                config->m_data_offset += param_type & 0x0F;
            }
            return true;
        }

        bool set_string(config_t* config, s16 id, const str_t& str)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT) || str_len(str) >= SETTING_STRING_MAX_LENGTH)
                return false;

            if (config->m_param_types[id] == PARAM_TYPE_NONE)
            {
                if (init_param(config, id, PARAM_TYPE_STRING) == false)
                    return false;
            }
            if (config->m_param_types[id] != PARAM_TYPE_NONE)
                return false;
            const u8 str_index          = config->m_param_value_offset[id];
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
            s32 const index = config->m_param_value_offset[id];
            if (index < 0 || index >= config->m_string_count)
                return false;
            s32 const   str_length = config->m_strlen[index];
            const char* str_data   = &config->m_strings[index * SETTING_STRING_MAX_LENGTH];
            outStr                 = str_const_full(str_data, 0, str_length, str_length);
            return true;
        }

        bool set_int8(config_t* config, s16 id, s8 value)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return false;
            if (config->m_param_types[id] == PARAM_TYPE_NONE)
            {
                if (init_param(config, id, PARAM_TYPE_S8) == false)
                    return false;  // No more space for u8 values
            }
            if (config->m_param_types[id] != PARAM_TYPE_S8)
                return false;
            const u8 offset = config->m_param_value_offset[id];
            write_u8(config->m_param_value_data, offset, (u8)value);
            return true;
        }

        bool get_int8(const config_t* config, s16 id, s8& outValue)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT) || config->m_param_types[id] != PARAM_TYPE_S8)
                return false;
            const u8 offset = config->m_param_value_offset[id];
            outValue        = (s8)read_u8(config->m_param_value_data, offset);
            return true;
        }

        bool set_uint8(config_t* config, s16 id, u8 value)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return false;
            if (config->m_param_types[id] == PARAM_TYPE_NONE)
            {
                if (init_param(config, id, PARAM_TYPE_U8) == false)
                    return false;  // No more space for u8 values
            }
            if (config->m_param_types[id] != PARAM_TYPE_U8)
                return false;
            const u8 offset = config->m_param_value_offset[id];
            write_u8(config->m_param_value_data, offset, value);
            return true;
        }

        bool get_uint8(const config_t* config, s16 id, u8& outValue)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT) || config->m_param_types[id] != PARAM_TYPE_U8)
                return false;
            const u8 offset = config->m_param_value_offset[id];
            outValue        = read_u8(config->m_param_value_data, offset);
            return true;
        }

        bool set_int16(config_t* config, s16 id, s16 value)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return false;
            if (config->m_param_types[id] == PARAM_TYPE_NONE)
            {
                if (init_param(config, id, PARAM_TYPE_S16) == false)
                    return false;
            }
            if (config->m_param_types[id] != PARAM_TYPE_S16)
                return false;
            const u8 offset = config->m_param_value_offset[id];
            write_u16(config->m_param_value_data, offset, (u16)value);
            return true;
        }

        bool get_int16(const config_t* config, s16 id, s16& outValue)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT) || config->m_param_types[id] != PARAM_TYPE_S16)
                return false;
            const u8 offset = config->m_param_value_offset[id];
            outValue        = (s16)read_u16(config->m_param_value_data, offset);
            return true;
        }

        bool set_uint16(config_t* config, s16 id, u16 value)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return false;
            if (config->m_param_types[id] == PARAM_TYPE_NONE)
            {
                if (init_param(config, id, PARAM_TYPE_U16) == false)
                    return false;
            }
            if (config->m_param_types[id] != PARAM_TYPE_U16)
                return false;
            const u8 offset = config->m_param_value_offset[id];
            write_u16(config->m_param_value_data, offset, value);
            return true;
        }

        bool get_uint16(const config_t* config, s16 id, u16& outValue)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT) || config->m_param_types[id] != PARAM_TYPE_U16)
                return false;
            const u8 offset = config->m_param_value_offset[id];
            outValue        = read_u16(config->m_param_value_data, offset);
            return true;
        }

        bool set_uint64(config_t* config, s16 id, u64 value)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT))
                return false;
            if (config->m_param_types[id] == PARAM_TYPE_NONE)
            {
                if (init_param(config, id, PARAM_TYPE_U64) == false)
                    return false;
            }
            if (config->m_param_types[id] != PARAM_TYPE_U64)
                return false;
            const u8 offset = config->m_param_value_offset[id];
            write_u64(config->m_param_value_data, offset, value);
            return true;
        }

        bool get_uint64(const config_t* config, s16 id, u64& outValue)
        {
            if (config == nullptr || (id < 0 || id >= SETTING_PARAM_MAX_COUNT) || config->m_param_types[id] != PARAM_TYPE_U64)
                return false;
            const u8 offset = config->m_param_value_offset[id];
            outValue        = read_u64(config->m_param_value_data, offset);
            return true;
        }
    }  // namespace nconfig
}  // namespace ncore