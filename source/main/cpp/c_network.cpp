#include "rdno_core/c_network.h"
#include "rdno_core/c_str.h"

namespace ncore
{
    // examples of valid IP string addresses:
    // "10.0.0.1"
    // "192.168.4.1"
    // examples of invalid IP string addresses:
    // "256.0.0.1"   (256 is out of range)
    // "192.168.4"   (only 3 parts)
    // "10.001.1.1   (leading zero)

    bool from_string(str_t const& str, IPAddress_t& outAddr)
    {
        s32 partIndex = 0;
        s32 partValue = 0;
        s32 partChars = 0;

        str_t iter = str;
        while (iter.m_str <= iter.m_end)
        {
            const char ch = iter.m_str < iter.m_end ? iter.m_const[iter.m_str] : '.';
            if (ch == '.')
            {
                if (partChars == 0 || partChars > 3 ||  partIndex >= 4 || partValue > 255)
                    return false; // Invalid format or too many parts
                if ((partValue <= 9 && partChars != 1) || (partValue > 9 && partValue <= 99 && partChars != 2))
                    return false; // Leading zero is not allowed
                outAddr.m_address[partIndex++] = (byte)partValue;
                partValue                      = 0;
                partChars                      = 0;
            }
            else
            {
                const byte digit = from_char(ch);
                if (digit > 9)
                    return false; // Invalid character
                partValue = partValue * 10 + digit;
                partChars++;
            }
            iter.m_str++;
        }

        return (partIndex == 4);
    }

    bool from_string(str_t const& str, MACAddress_t& outAddr)
    {
        s32 partIndex = 0;
        s32 partValue = 0;
        s32 partChars = 0;

        str_t iter = str;
        while (iter.m_str <= iter.m_end)
        {
            const char ch = iter.m_str < iter.m_end ? iter.m_const[iter.m_str] : ':';
            if (ch == ':')
            {
                if (partValue > 255 || partChars == 0 || partChars > 2 || partIndex >= 6)
                    return false; // Invalid format, too little or too many parts
                outAddr.m_address[partIndex++] = partValue;
                partValue                      = 0;
                partChars                      = 0;
            }
            else if (is_hexa(ch))
            {
                partValue = (partValue << 4) | from_char(ch);
                partChars++;
            }
            else
            {
                return false; // Invalid character
            }

            iter.m_str++;
        }

        return (partIndex == 6);
    }

    bool is_valid_SSID(str_t const& str)
    {
        str_t iter = str;
        while (iter.m_str < iter.m_end)
        {
            const char ch = iter.m_const[iter.m_str];
            if (ch < 32 || ch > 126) // Printable ASCII range
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
            if (ch < 32 || ch > 126) // Printable ASCII range
                return false;
            iter.m_str++;
        }
        const s16 length = (s16)(iter.m_str - str.m_str);
        return length >= 8 && length <= 64;
    }

    bool is_valid_IPAddress(str_t const& str)
    {
        IPAddress_t addr;
        return from_string(str, addr);
    }

    bool is_valid_port(s32 port)
    {
        return port > 1023 && port <= 65535;
    }

} // namespace ncore
