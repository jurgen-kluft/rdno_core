#include "rdno_core/c_target.h"
#include "rdno_core/c_allocator.h"
#include "rdno_core/c_str.h"
#include "rdno_core/c_config.h"
#include "rdno_core/c_memory.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(config)
{
    UNITTEST_FIXTURE(general)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        s16 KeyToIndex(str_t const& str)
        {
            if (str_cmp_n(str, "ssid", 4, false) == 0)
                return nconfig::PARAM_ID_WIFI_SSID;
            if (str_cmp_n(str, "pw", 2, false) == 0)
                return nconfig::PARAM_ID_WIFI_PASSWORD;
            if (str_cmp_n(str, "ap_ssid", 7, false) == 0)
                return nconfig::PARAM_ID_AP_SSID;
            if (str_cmp_n(str, "ap_pw", 5, false) == 0)
                return nconfig::PARAM_ID_AP_PASSWORD;
            if (str_cmp_n(str, "server", 6, false) == 0)
                return nconfig::PARAM_ID_REMOTE_SERVER;
            if (str_cmp_n(str, "port", 4, false) == 0)
                return nconfig::PARAM_ID_REMOTE_PORT;

            s32 value = 0;
            if (from_str(str, &value, 10) && value >= 0 && value < nconfig::SETTING_ID_MAX_COUNT)
            {
                return static_cast<s16>(value);
            }
            return -1;
        }

        UNITTEST_TEST(parse_message_1)
        {
            const char* message = "ssid=OBNOSIS8, pw=MySecretPassword, server=10.0.0.62, port=1234";

            nconfig::config_t config;
            nconfig::reset(&config);
            nconfig::set_string(&config, nconfig::PARAM_ID_WIFI_SSID, str_const(""));      // ssid
            nconfig::set_string(&config, nconfig::PARAM_ID_WIFI_PASSWORD, str_const(""));  // password
            nconfig::set_string(&config, nconfig::PARAM_ID_AP_SSID, str_const(""));        // ap_ssid
            nconfig::set_string(&config, nconfig::PARAM_ID_AP_PASSWORD, str_const(""));    // ap_password
            nconfig::set_string(&config, nconfig::PARAM_ID_REMOTE_SERVER, str_const(""));  // remote_server
            nconfig::set_int16(&config, nconfig::PARAM_ID_REMOTE_PORT, 12);                  // remote_port
            CHECK_EQUAL(5, config.m_param_values[nconfig::PARAM_ID_STRING_COUNT]);         // 5 strings used so far

            str_t msg      = str_const(message);
            str_t outKey   = str_empty();
            str_t outValue = str_empty();
            while (nconfig::parse_keyvalue(msg, outKey, outValue))
            {
                s16 id = KeyToIndex(outKey);
                CHECK_TRUE(id >= 0);
                nconfig::parse_value(&config, id, outValue);
            }

            str_t ssid;
            CHECK_TRUE(nconfig::get_string(&config, nconfig::PARAM_ID_WIFI_SSID, ssid));
            CHECK_TRUE(str_eq(ssid, "OBNOSIS8", false));

            str_t password;
            CHECK_TRUE(nconfig::get_string(&config, nconfig::PARAM_ID_WIFI_PASSWORD, password));
            CHECK_TRUE(str_eq(password, "MySecretPassword", false));

            str_t remote_server;
            CHECK_TRUE(nconfig::get_string(&config, nconfig::PARAM_ID_REMOTE_SERVER, remote_server));
            CHECK_TRUE(str_eq(remote_server, "10.0.0.62", false));

            CHECK_TRUE(config.m_param_types[nconfig::PARAM_ID_REMOTE_PORT] == nconfig::PARAM_TYPE_S16);

            s16 remote_port;
            CHECK_TRUE(nconfig::get_int16(&config, nconfig::PARAM_ID_REMOTE_PORT, remote_port));
            CHECK_EQUAL(1234, remote_port);
        }
    }
}
UNITTEST_SUITE_END
