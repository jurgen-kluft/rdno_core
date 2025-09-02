#include "rdno_core/c_target.h"
#include "rdno_core/c_allocator.h"
#include "rdno_core/c_str.h"
#include "rdno_core/c_nvstore.h"
#include "rdno_core/c_memory.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(nvstore)
{
    UNITTEST_FIXTURE(general)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        s16 KeyToIndex(str_t const& str)
        {
            if (str_len(str) == 4 && str_cmp_n(str, "ssid", 4, false) == 0)
                return 0;
            if (str_len(str) == 8 && str_cmp_n(str, "password", 8, false) == 0)
                return 1;
            if (str_len(str) == 7 && str_cmp_n(str, "ap_ssid", 7, false) == 0)
                return 2;
            if (str_len(str) == 11 && str_cmp_n(str, "ap_password", 11, false) == 0)
                return 3;
            if (str_len(str) == 13 && str_cmp_n(str, "remote_server", 13, false) == 0)
                return 4;
            if (str_len(str) == 11 && str_cmp_n(str, "remote_port", 11, false) == 0)
                return 5;

            s32 value = 0;
            if (from_str(str, &value, 10) && value >= 0 && value < nvstore::PARAM_ID_MAX_COUNT)
            {
                return static_cast<s16>(value);
            }
            return -1;
        }

        UNITTEST_TEST(parse_message_1)
        {
            const char* message = "ssid=OBNOSIS8, password=MySecretPassword, remote_server=10.0.0.62, remote_port=1234";

            nvstore::config_t config;
            nvstore::reset(&config);
            nvstore::set_string(&config, 0, str_const(""));                          // ssid
            nvstore::set_string(&config, 1, str_const(""));                          // password
            nvstore::set_string(&config, 2, str_const(""));                          // ap_ssid
            nvstore::set_string(&config, 3, str_const(""));                          // ap_password
            nvstore::set_string(&config, 4, str_const(""));                          // remote_server
            nvstore::set_int(&config, 5, 12);                                        // remote_port
            CHECK_EQUAL(5, config.m_param_values[nvstore::PARAM_ID_STRING_COUNT]);  // 5 strings used so far

            str_t msg      = str_const(message);
            str_t outKey   = str_empty();
            str_t outValue = str_empty();
            while (nvstore::parse_keyvalue(msg, outKey, outValue))
            {
                s16 id = KeyToIndex(outKey);
                CHECK_TRUE(id >= 0);
                nvstore::parse_value(&config, id, outValue);
            }

            str_t ssid;
            CHECK_TRUE(nvstore::get_string(&config, 0, ssid));
            CHECK_TRUE(str_eq(ssid, "OBNOSIS8", false));

            str_t password;
            CHECK_TRUE(nvstore::get_string(&config, 1, password));
            CHECK_TRUE(str_eq(password, "MySecretPassword", false));

            str_t remote_server;
            CHECK_TRUE(nvstore::get_string(&config, 4, remote_server));
            CHECK_TRUE(str_eq(remote_server, "10.0.0.62", false));

            CHECK_TRUE(config.m_param_types[5] == nvstore::PARAM_TYPE_S32);

            s32 remote_port = nvstore::get_int(&config, 5, 123);
            CHECK_EQUAL(1234, remote_port);
        }
    }
}
UNITTEST_SUITE_END
