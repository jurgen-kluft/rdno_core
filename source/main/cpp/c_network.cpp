#ifdef TARGET_ARDUINO
#    include "Arduino.h"
#endif
#ifdef TARGET_ESP8266
#    include "IPAddress.h"
#endif

#include "rdno_core/c_str.h"
#include "rdno_core/c_network.h"

namespace ncore
{
#ifndef TARGET_ARDUINO
    void IPAddress_t::reset()
    {
        for (s32 i = 0; i < 16; ++i)
        {
            _address.bytes[i] = 0;
        }
        type = IPv4;
    }

    IPAddress_t IPAddress_t::from(u32 _ip)
    {
        IPAddress_t ip;
        ip._address.bytes[12 + 0] = (_ip >> 24) & 0xFF;
        ip._address.bytes[12 + 1] = (_ip >> 16) & 0xFF;
        ip._address.bytes[12 + 2] = (_ip >> 8) & 0xFF;
        ip._address.bytes[12 + 3] = (_ip >> 0) & 0xFF;
        ip.type                   = IPv4;
        return ip;
    }

    IPAddress_t IPAddress_t::from(u8 a, u8 b, u8 c, u8 d)
    {
        IPAddress_t ip;
        ip._address.bytes[12 + 0] = a;
        ip._address.bytes[12 + 1] = b;
        ip._address.bytes[12 + 2] = c;
        ip._address.bytes[12 + 3] = d;
        ip.type                   = IPv4;
        return ip;
    }
#endif

    // examples of valid IP string addresses:
    // "10.0.0.1"
    // "192.168.4.1"
    // examples of invalid IP string addresses:
    // "256.0.0.1"   (256 is out of range)
    // "192.168.4"   (only 3 parts)
    // "10.001.1.1   (leading zero)

    bool from_string(str_t const& str, IPAddress_t& outAddr)
    {
#ifdef TARGET_ARDUINO
        return outAddr.fromString(str.m_const);
#else
        s32 partIndex = 0;
        u32 partValue = 0;
        s32 partChars = 0;

        str_t iter = str;
        while (iter.m_str <= iter.m_end)
        {
            const char ch = iter.m_str < iter.m_end ? iter.m_const[iter.m_str] : '.';
            if (ch == '.')
            {
                if (partChars == 0 || partChars > 3 || partIndex >= 4 || partValue > 255)
                    return false;  // Invalid format or too many parts
                if ((partValue <= 9 && partChars != 1) || (partValue > 9 && partValue <= 99 && partChars != 2))
                    return false;  // Leading zero is not allowed
                outAddr[partIndex] = (partValue << ((3 - partIndex) * 8));
                partValue          = 0;
                partChars          = 0;
                partIndex++;
            }
            else
            {
                const byte digit = from_char(ch);
                if (digit > 9)
                    return false;  // Invalid character
                partValue = partValue * 10 + digit;
                partChars++;
            }
            iter.m_str++;
        }

        if (partIndex == 4)
        {
            // Move to IPv4
            outAddr.type = IPAddress_t::IPv4;
            ip[12]       = ip[0];
            ip[13]       = ip[1];
            ip[14]       = ip[2];
            ip[15]       = ip[3];
            ip[0]        = 0;
            ip[1]        = 0;
            ip[2]        = 0;
            ip[3]        = 0;
            return true;
        }
        else if (partIndex == 16)
        {
            // IPv6
            outAddr.type = IPAddress_t::IPv6;
            return true;
        }
        return false;  // Not enough parts
#endif
    }

    bool from_string(str_t const& str, u8* outAddr)
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
                    return false;  // Invalid format, too little or too many parts
                outAddr[partIndex++] = partValue;
                partValue            = 0;
                partChars            = 0;
            }
            else if (is_hexa(ch))
            {
                partValue = (partValue << 4) | from_char(ch);
                partChars++;
            }
            else
            {
                return false;  // Invalid character
            }

            iter.m_str++;
        }

        return (partIndex == 6);
    }

    void to_str(str_t& str, const IPAddress_t& address, const char* sep)
    {
        to_str(str, (u32)address[0], 10);
        str_append(str, sep);
        to_str(str, (u32)(address[1]) & 0xFF, 10);
        str_append(str, sep);
        to_str(str, (u32)(address[2]) & 0xFF, 10);
        str_append(str, sep);
        to_str(str, (u32)(address[3]) & 0xFF, 10);
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

    bool is_valid_IPAddress(IPAddress_t const& addr)
    {
#ifdef TARGET_ESP8266
        return addr.isSet();
#else
        // Exclude special ranges: 0.x.x.x, 127.x.x.x, 224.x.x.x - 255.x.x.x
        if (addr.type() == IPv4)
        {
            return addr[0] != 0 && addr[0] != 127 && (addr[0] < 224);
        }
        else
        {
            // For IPv6, we can add more checks if needed
            return true;  // Accept all IPv6 addresses for now
        }
#endif
    }

    bool is_valid_IPAddress(str_t const& str)
    {
        IPAddress_t addr;
        return from_string(str, addr);
    }

    bool is_valid_port(u16 port) { return port > 1023 && port <= 65535; }

}  // namespace ncore
