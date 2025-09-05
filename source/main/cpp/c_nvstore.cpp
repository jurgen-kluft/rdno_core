#include "rdno_core/c_nvstore.h"
#include "rdno_core/c_memory.h"
#include "rdno_core/c_serial.h"
#include "rdno_core/c_str.h"

#ifdef TARGET_ESP32

#    include "nvs_flash.h"
#    include "nvs.h"

namespace ncore
{
    namespace nvstore
    {
        bool s_initialized = false;
        bool s_valid       = false;

        // List of all nvs esp errors:
        // ESP_ERR_NVS_NOT_INITIALIZED
        // ESP_ERR_NVS_NOT_FOUND
        // ESP_ERR_NVS_TYPE_MISMATCH
        // ESP_ERR_NVS_READ_ONLY
        // ESP_ERR_NVS_NOT_ENOUGH_SPACE
        // ESP_ERR_NVS_INVALID_NAME
        // ESP_ERR_NVS_INVALID_HANDLE
        // ESP_ERR_NVS_REMOVE_FAILED
        // ESP_ERR_NVS_KEY_TOO_LONG
        // ESP_ERR_NVS_PAGE_FULL
        // ESP_ERR_NVS_INVALID_STATE
        // ESP_ERR_NVS_INVALID_LENGTH
        // ESP_ERR_NVS_NO_FREE_PAGES
        // ESP_ERR_NVS_VALUE_TOO_LONG
        // ESP_ERR_NVS_PART_NOT_FOUND
        // ESP_ERR_NVS_NEW_VERSION_FOUND
        // ESP_ERR_NVS_XTS_ENCR_FAILED
        // ESP_ERR_NVS_XTS_DECR_FAILED
        // ESP_ERR_NVS_XTS_CFG_FAILED
        // ESP_ERR_NVS_XTS_CFG_NOT_FOUND
        // ESP_ERR_NVS_ENCR_NOT_SUPPORTED
        // ESP_ERR_NVS_KEYS_NOT_INITIALIZED
        // ESP_ERR_NVS_CORRUPT_KEY_PART
        // ESP_ERR_NVS_WRONG_ENCRYPTION

        static void print_nvs_error(const char* cmd, esp_err_t err)
        {
            switch (err)
            {
                case ESP_OK: break;
                case ESP_ERR_NVS_NOT_INITIALIZED:
                    nserial::print(cmd);
                    nserial::println("NVS not initialized");
                    break;
                case ESP_ERR_NVS_NOT_FOUND:
                    nserial::print(cmd);
                    nserial::println("NVS not found");
                    break;
                case ESP_ERR_NVS_TYPE_MISMATCH:
                    nserial::print(cmd);
                    nserial::println("NVS type mismatch");
                    break;
                case ESP_ERR_NVS_READ_ONLY:
                    nserial::print(cmd);
                    nserial::println("NVS read only");
                    break;
                case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
                    nserial::print(cmd);
                    nserial::println("NVS not enough space");
                    break;
                case ESP_ERR_NVS_INVALID_NAME:
                    nserial::print(cmd);
                    nserial::println("NVS invalid name");
                    break;
                case ESP_ERR_NVS_INVALID_HANDLE:
                    nserial::print(cmd);
                    nserial::println("NVS invalid handle");
                    break;
                case ESP_ERR_NVS_REMOVE_FAILED:
                    nserial::print(cmd);
                    nserial::println("NVS remove failed");
                    break;
                case ESP_ERR_NVS_KEY_TOO_LONG:
                    nserial::print(cmd);
                    nserial::println("NVS key too long");
                    break;
                case ESP_ERR_NVS_PAGE_FULL:
                    nserial::print(cmd);
                    nserial::println("NVS page full");
                    break;
                case ESP_ERR_NVS_INVALID_STATE:
                    nserial::print(cmd);
                    nserial::println("NVS invalid state");
                    break;
                case ESP_ERR_NVS_INVALID_LENGTH:
                    nserial::print(cmd);
                    nserial::println("NVS invalid length");
                    break;
                case ESP_ERR_NVS_NO_FREE_PAGES:
                    nserial::print(cmd);
                    nserial::println("NVS no free pages");
                    break;
                case ESP_ERR_NVS_VALUE_TOO_LONG:
                    nserial::print(cmd);
                    nserial::println("NVS value too long");
                    break;
                case ESP_ERR_NVS_PART_NOT_FOUND:
                    nserial::print(cmd);
                    nserial::println("NVS part not found");
                    break;
                case ESP_ERR_NVS_NEW_VERSION_FOUND:
                    nserial::print(cmd);
                    nserial::println("NVS new version found");
                    break;
                case ESP_ERR_NVS_XTS_ENCR_FAILED:
                    nserial::print(cmd);
                    nserial::println("NVS XTS encr failed");
                    break;
                case ESP_ERR_NVS_XTS_DECR_FAILED:
                    nserial::print(cmd);
                    nserial::println("NVS XTS decr failed");
                    break;
                case ESP_ERR_NVS_XTS_CFG_FAILED:
                    nserial::print(cmd);
                    nserial::println("NVS XTS cfg failed");
                    break;
                case ESP_ERR_NVS_XTS_CFG_NOT_FOUND:
                    nserial::print(cmd);
                    nserial::println("NVS XTS cfg not found");
                    break;
                case ESP_ERR_NVS_ENCR_NOT_SUPPORTED:
                    nserial::print(cmd);
                    nserial::println("NVS encr not supported");
                    break;
                case ESP_ERR_NVS_KEYS_NOT_INITIALIZED:
                    nserial::print(cmd);
                    nserial::println("NVS keys not initialized");
                    break;
                case ESP_ERR_NVS_CORRUPT_KEY_PART:
                    nserial::print(cmd);
                    nserial::println("NVS corrupt key part");
                    break;
                case ESP_ERR_NVS_WRONG_ENCRYPTION:
                    nserial::print(cmd);
                    nserial::println("NVS wrong encryption");
                    break;
                default: nserial::println("NVS unknown error");
            }
        }

        static bool initialize()
        {
            if (!s_initialized)
            {
                esp_err_t err = nvs_flash_init();
                if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
                {
                    print_nvs_error("nvs_flash_init, error = ", err);

                    // NVS partition was truncated and needs to be erased
                    // Retry nvs_flash_init
                    err = nvs_flash_erase();
                    print_nvs_error("nvs_flash_erase, error = ", err);
                    if (err == ESP_OK)
                    {
                        err = nvs_flash_init();
                        print_nvs_error("nvs_flash_init, error = ", err);
                    }
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
            nserial::println("NVS open to save");
            nvs_handle_t storage_handle;
            esp_err_t    err = nvs_open("node_config", NVS_READWRITE, &storage_handle);
            print_nvs_error("nvs_open, error = ", err);
            if (err == ESP_OK)
            {
                nserial::println("NVS save config");
                err = nvs_set_blob(storage_handle, "config_t", (const void*)config, sizeof(config_t));
                print_nvs_error("nvs_set_blob, error = ", err);
                err = nvs_commit(storage_handle);
                print_nvs_error("nvs_commit, error = ", err);
                nvs_close(storage_handle);
            }
        }

        bool load(config_t* config)
        {
            if (!initialize())
                return false;

            nvs_handle_t storage_handle;
            esp_err_t    err = nvs_open("node_config", NVS_READONLY, &storage_handle);
            print_nvs_error("nvs_open, error = ", err);
            if (err == ESP_OK)
            {
                size_t required_size;
                err = nvs_get_blob(storage_handle, "config_t", nullptr, &required_size);
                print_nvs_error("nvs_get_blob1, error = ", err);
                if (err == ESP_OK && required_size == sizeof(config_t))
                {
                    err = nvs_get_blob(storage_handle, "config_t", (void*)config, &required_size);
                    print_nvs_error("nvs_get_blob2, error = ", err);
                }
                else
                {
                    err = ESP_ERR_NVS_INVALID_LENGTH;
                }

                if (err != ESP_OK)
                    return false;

                nvs_close(storage_handle);
                return true;
            }
            return false;
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
        bool load(config_t* config) { return true; }

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