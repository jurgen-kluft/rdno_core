#include "rdno_core/c_nvstore.h"
#include "rdno_core/c_memory.h"
#include "rdno_core/c_str.h"

#ifdef TARGET_ESP32

#    include "rdno_core/c_memory.h"

#    include "nvs_flash.h"
#    include "nvs.h"

namespace ncore
{
    namespace nvstore
    {
        bool s_initialized = false;
        bool s_valid       = false;

        static bool initialize()
        {
            if (!s_initialized)
            {
                esp_err_t err = nvs_flash_init();
                if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
                {
                    // NVS partition was truncated and needs to be erased
                    // Retry nvs_flash_init
                    err = nvs_flash_erase();
                    err = (err == ESP_OK) && nvs_flash_init();
                }
                s_initialized = true;
                s_valid       = (err == ESP_OK);
            }
            return s_valid;
        }

        void reset(config_t* config) { g_memset(config, 0, sizeof(config_t)); }

        void save(config_t* config)
        {
            if (!initialize())
                return;

            // write to non-volatile storage
            nvs_handle my_handle;
            esp_err_t  err = nvs_open("storage", NVS_READWRITE, &my_handle);
            if (err == ESP_OK)
            {
                err = nvs_set_blob(my_handle, "nvs_struct", (const void*)&config, sizeof(config_t));
                nvs_commit(my_handle);
                nvs_close(my_handle);
            }
        }

        void load(config_t* config)
        {
            if (!initialize())
                return;

            nvs_handle my_handle;
            esp_err_t  err = nvs_open("storage", NVS_READWRITE, &my_handle);
            if (err == ESP_OK)
            {
                size_t required_size;
                err = nvs_get_blob(my_handle, "nvs_struct", nullptr, &required_size);
                if (err == ESP_OK && required_size == sizeof(config_t))
                {
                    err = nvs_get_blob(my_handle, "nvs_struct", (void*)config, &required_size);
                }
                else
                {
                    err = ESP_ERR_NVS_INVALID_LENGTH;
                }

                if (err != ESP_OK)
                    g_memset(config, 0, sizeof(config_t));

                nvs_close(my_handle);
            }
        }

    }  // namespace nvstore
}  // namespace ncore

#else

namespace ncore
{
    namespace nvstore
    {
        void reset(config_t* config) { g_memset(config, 0, sizeof(config_t)); }
        void save(config_t* config) {}
        void load(config_t* config) {}

    }  // namespace nvstore
}  // namespace ncore

#endif

namespace ncore
{
    namespace nvstore
    {
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
                case nvstore::PARAM_TYPE_NONE: break;
                case nvstore::PARAM_TYPE_STRING: nvstore::set_string(config, id, str); break;
                case nvstore::PARAM_TYPE_S32: nvstore::parse_int(config, id, str); break;
                case nvstore::PARAM_TYPE_BOOL: nvstore::parse_bool(config, id, str); break;
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
                config->m_param_values[id] = (str_index * PARAM_ID_STRING_MAX_LENGTH) + str_len(str);
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
            s32 const str_index  = str_value / PARAM_ID_STRING_MAX_LENGTH;
            s32 const str_length = str_value & (PARAM_ID_STRING_MAX_LENGTH - 1);
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

        s32 get_int(const config_t* config, s16 id, s32 defaultValue)
        {
            if (config == nullptr || (id < 0 || id >= PARAM_ID_MAX_COUNT) || config->m_param_types[id] != PARAM_TYPE_S32)
                return defaultValue;
            return config->m_param_values[id];
        }

        void set_bool(config_t* config, s16 id, bool value)
        {
            if (config == nullptr || (id < 0 || id >= PARAM_ID_MAX_COUNT))
                return;
            config->m_param_types[id]  = PARAM_TYPE_BOOL;
            config->m_param_values[id] = value ? 1 : 0;
        }

        bool get_bool(const config_t* config, s16 id, bool defaultValue)
        {
            if (config == nullptr || (id < 0 || id >= PARAM_ID_MAX_COUNT) || config->m_param_types[id] != PARAM_TYPE_BOOL)
                return defaultValue;
            return config->m_param_values[id] == 1;
        }

    }  // namespace nvstore
}  // namespace ncore