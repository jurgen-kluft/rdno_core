#include "rdno_core/c_nvstore.h"

#ifdef TARGET_ESP32

#    include "Arduino.h"
#    include "nvs_flash.h"
#    include "nvs.h"

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
                    memset(config, 0, sizeof(config_t));

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
        bool Load(config_t* config) { return true; }

    }  // namespace nvstore
}  // namespace ncore

#endif
