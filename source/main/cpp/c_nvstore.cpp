#include "rdno_core/c_nvstore.h"
#include "rdno_core/c_str.h"

#ifdef TARGET_ESP32

#    include "rdno_core/c_memory.h"

#    include "nvs_flash.h"
#    include "nvs.h"
#    include <string.h>

namespace ncore
{
    namespace nvstore
    {
        bool s_initialized = false;
        bool s_valid       = false;

        bool Initialize()
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

        void Save(config_t* config)
        {
            if (!Initialize())
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

        void Load(config_t* config)
        {
            if (!Initialize())
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
        void Save(config_t* config) {}
        void Load(config_t* config) {}

    }  // namespace nvstore
}  // namespace ncore

#endif

namespace ncore
{
    namespace nvstore
    {
        static const char* SkipWhitespace(const char* str, const char* end)
        {
            while (str < end && (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n'))
                str++;
            return str;
        }

        static const char* FindChar(const char* str, const char* end, char ch)
        {
            while (str < end && *str != ch)
                str++;
            return str;
        }

        // Message example: "ssid=OBNOSIS8, password=MySecretPassword, remote_server=10.0.0.22, remote_port=1234"
        bool ParseKeyValue(const char*& msg, const char* msgEnd, const char*& outKey, s32& outKeyLength, const char*& outValue, s32& outValueLength)
        {
            outKey = SkipWhitespace(msg, msgEnd);
            if (outKey == msgEnd)
                return false;  // No more key-value pairs
            const char* equalSign = FindChar(outKey, msgEnd, '=');
            if (equalSign == msgEnd)
                return false;  // No '=' found
            outKeyLength = static_cast<s32>(equalSign - outKey);

            outValue = SkipWhitespace(equalSign + 1, msgEnd);
            if (outValue == msgEnd)
                return false;  // No value found
            const char* comma = FindChar(outValue, msgEnd, ',');
            outValueLength    = static_cast<s32>((comma == msgEnd ? msgEnd : comma) - outValue);

            msg = (comma == msgEnd) ? msgEnd : SkipWhitespace(comma + 1, msgEnd);
            return msg < msgEnd;
        }

        void ParseValue(config_t* config, s16 id, const char* str, s32 len)
        {
            if (config == nullptr || (id < 0 || id >= 63))
                return;
            switch (config->m_params[id].m_type)
            {
                case nvstore::TYPE_NONE: break;
                case nvstore::TYPE_STRING: nvstore::SetString(config, id, str, len); break;
                case nvstore::TYPE_S32: nvstore::ParseInt(config, id, str, len); break;
                case nvstore::TYPE_BOOL: nvstore::ParseBool(config, id, str, len); break;
            }
        }

        void ParseInt(config_t* config, s16 id, const char* str, s32 len)
        {
            if (config == nullptr || (id < 0 || id >= 63))
                return;
            str_t s = str_const_n(str, 0, len, len);

            s32 value = 0;
            from_str(s, &value, 10);

            SetInt(config, id, value);
        }

        void ParseBool(config_t* config, s16 id, const char* str, s32 len)
        {
            if (config == nullptr || (id < 0 || id >= 63))
                return;

            str_t s = str_const_n(str, 0, len, len);
            if (str_eq(s, "true") || str_eq(s, "1"))
            {
                SetBool(config, id, true);
            }
            else
            {
                SetBool(config, id, false);
            }
        }

        void SetString(config_t* config, s16 id, const char* str, s32 strLen)
        {
            if (config == nullptr || (id < 0 || id >= 63))
                return;

            s32 str_index;
            if (config->m_params[id].m_type == TYPE_NONE)
            {
                if (config->m_string_count >= 32)
                {
                    return;  // No more space for strings
                }

                str_index                    = config->m_string_count++;
                config->m_params[id].m_type  = TYPE_STRING;
                config->m_params[id].m_value = str_index;
            }
            else
            {
                str_index = config->m_params[id].m_value;
            }

            str_t src = str_const_n(str, 0, strLen, strLen);
            str_t dst = str_mutable(&config->m_strings[str_index * 32], 32);
            str_append(dst, src);
        }

        const char* GetString(const config_t* config, s16 id)
        {
            if (config == nullptr || (id < 0 || id >= 63))
                return "";
            if (config->m_params[id].m_type != TYPE_STRING)
                return "";
            s32 const str_index = config->m_params[id].m_value;
            if (str_index < 0 || str_index >= config->m_string_count)
                return "";
            return &config->m_strings[str_index * 32];
        }
        void SetInt(config_t* config, s16 id, s32 value)
        {
            if (config == nullptr || (id < 0 || id >= 63))
                return;
            config->m_params[id].m_type  = TYPE_S32;
            config->m_params[id].m_value = value;
        }
        s32 GetInt(const config_t* config, s16 id, s32 defaultValue)
        {
            if (config == nullptr || (id < 0 || id >= 63) || config->m_params[id].m_type != TYPE_S32)
                return defaultValue;
            return config->m_params[id].m_value;
        }
        void SetBool(config_t* config, s16 id, bool value)
        {
            if (config == nullptr || (id < 0 || id >= 63))
                return;
            config->m_params[id].m_type  = TYPE_BOOL;
            config->m_params[id].m_value = value ? 1 : 0;
        }
        bool GetBool(const config_t* config, s16 id, bool defaultValue)
        {
            if (config == nullptr || (id < 0 || id >= 63) || config->m_params[id].m_type != TYPE_BOOL)
                return defaultValue;
            return config->m_params[id].m_value == 1;
        }

    }  // namespace nvstore
}  // namespace ncore