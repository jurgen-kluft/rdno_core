#include "rcore/c_str.h"
#include "rcore/c_network.h"

namespace ncore
{
    void IPAddress_t::reset()
    {
        flags = 0;
    }


    void to_str(str_t& str, const u8* mac, const char* sep)
    {
        to_str(str, mac[0], 2, 16);
        str_append(str, sep);
        to_str(str, mac[1], 2, 16);
        str_append(str, sep);
        to_str(str, mac[2], 2, 16);
        str_append(str, sep);
        to_str(str, mac[3], 2, 16);
        str_append(str, sep);
        to_str(str, mac[4], 2, 16);
        str_append(str, sep);
        to_str(str, mac[5], 2, 16);
    }

    bool is_valid_SSID(str_t const& str)
    {
        str_t iter = str;
        while (iter.m_str < iter.m_end)
        {
            const char ch = iter.m_const[iter.m_str];
            if (ch < 32 || ch > 126)  // Printable ASCII range
                return false;
            iter.m_str++;
        }
        const s16 length = (s16)(iter.m_str - str.m_str);
        return length > 0 && length <= 32;
    }

    bool is_valid_password(str_t const& str)
    {
        str_t iter = str;
        while (iter.m_str < iter.m_end)
        {
            const char ch = iter.m_const[iter.m_str];
            if (ch < 32 || ch > 126)  // Printable ASCII range
                return false;
            iter.m_str++;
        }
        const s16 length = (s16)(iter.m_str - str.m_str);
        return length >= 8 && length <= 64;
    }

    bool is_valid_IPAddress(str_t const& str)
    {
        IPAddress_t addr;
        return addr.from_string(str);
    }

    bool is_valid_port(u16 port) { return port > 1023 && port <= 65535; }

}  // namespace ncore
