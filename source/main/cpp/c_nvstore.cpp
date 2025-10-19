#include "rdno_core/c_config.h"
#include "rdno_core/c_nvstore.h"
#include "rdno_core/c_memory.h"
#include "rdno_core/c_serial.h"
#include "rdno_core/c_str.h"

#ifdef TARGET_ARDUINO

#    ifdef TARGET_ESP32

#        include "nvs_flash.h"
#        include "nvs.h"

namespace ncore
{
    namespace nvstore
    {
        bool s_initialized = false;
        bool s_valid       = false;

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

#        define NVS_NODE_KEY "node"
#        define NVS_BLOB_KEY "config"

        void save(nconfig::config_t* config)
        {
            if (!initialize())
                return;

            // write to non-volatile storage
            nserial::println("NVS open to save");
            nvs_handle_t storage_handle;
            esp_err_t    err = nvs_open(NVS_NODE_KEY, NVS_READWRITE, &storage_handle);
            print_nvs_error("nvs_open, error = ", err);
            if (err == ESP_OK)
            {
                nserial::println("NVS save config");
                err = nvs_set_blob(storage_handle, NVS_BLOB_KEY, (const void*)config, sizeof(nconfig::config_t));
                print_nvs_error("nvs_set_blob, error = ", err);
                err = nvs_commit(storage_handle);
                print_nvs_error("nvs_commit, error = ", err);
                nvs_close(storage_handle);
            }
        }

        bool load(nconfig::config_t* config)
        {
            if (!initialize())
                return false;

            nvs_handle_t storage_handle;
            esp_err_t    err = nvs_open(NVS_NODE_KEY, NVS_READONLY, &storage_handle);
            print_nvs_error("nvs_open, error = ", err);
            if (err == ESP_OK)
            {
                size_t required_size;
                err = nvs_get_blob(storage_handle, NVS_BLOB_KEY, nullptr, &required_size);
                print_nvs_error("nvs_get_blob1, error = ", err);
                if (err == ESP_OK && required_size == sizeof(nconfig::config_t))
                {
                    err = nvs_get_blob(storage_handle, NVS_BLOB_KEY, (void*)config, &required_size);
                    print_nvs_error("nvs_get_blob2, error = ", err);
                }
                else
                {
                    err = ESP_ERR_NVS_INVALID_LENGTH;
                }

                nvs_close(storage_handle);
            }
            return (err == ESP_OK);
        }

    }  // namespace nvstore
}  // namespace ncore

#    endif  // #ifdef TARGET_ESP32

#    ifdef TARGET_ESP8266

#include "Arduino.h"

namespace ncore
{
    namespace nvstore
    {
        #define RTC_BLOCK_SIZE 4
        #define MAX_TOTAL_RTC_MEMORY_SIZE 512

        // Offset from the first block in RTC memory. This offset is used to shift data toward the
        // highest RTC memory addresses.
        #define RTC_MEMORY_OFFSET (MAX_TOTAL_RTC_MEMORY_SIZE - sizeof(nconfig::config_t)) / RTC_BLOCK_SIZE

        void save(nconfig::config_t* config) 
        {
            config->m_crc = 0;
            config->m_crc = crc32(config, sizeof(nconfig::config_t));

            ESP.rtcUserMemoryWrite(RTC_MEMORY_OFFSET, (uint32_t*)config, sizeof(nconfig::config_t));
        }

        bool load(nconfig::config_t* config) 
        { 
            ESP.rtcUserMemoryRead(RTC_MEMORY_OFFSET, (uint32_t*)config, sizeof(nconfig::config_t));
            const u32 crc = config->m_crc;
            
            config->m_crc = 0;
            const u32 calculated_crc = crc32(config, sizeof(nconfig::config_t));

            return (crc == calculated_crc);
        }

    }  // namespace nvstore
}  // namespace ncore

#    endif  // #ifdef TARGET_ESP8266

#else

namespace ncore
{
    namespace nvstore
    {
        void reset(nconfig::config_t* config) { g_memset(config, 0, sizeof(nconfig::config_t)); }
        void save(nconfig::config_t* config) {}
        bool load(nconfig::config_t* config) { return true; }

    }  // namespace nvstore
}  // namespace ncore

#endif
