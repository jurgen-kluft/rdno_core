#include "rcore/c_ipaddress.h"
#include "rcore/c_str.h"

#ifdef TARGET_ARDUINO

#    include "IPAddress.h"

#    ifdef TARGET_ESP32
namespace ncore
{
    IPAddress_t IPAddress_t::from(u32 ip)
    {
        IPAddress_t addr;
        IPAddress   ipAddr(ip);
        IPAddress_t::from_arduino(addr, ipAddr);
        return addr;
    }

    IPAddress_t IPAddress_t::from(u8 a, u8 b, u8 c, u8 d)
    {
        IPAddress_t addr;
        IPAddress   ipAddr(a, b, c, d);
        IPAddress_t::from_arduino(addr, ipAddr);
        return addr;
    }

    void IPAddress_t::from_arduino(IPAddress_t& dest, const IPAddress& src)
    {
        IPAddress* raw = (IPAddress*)dest.bytes;
        *raw           = src;
        dest.flags     = (src.type() == IPv4) ? 4 : 6;
    }

    void IPAddress_t::to_arduino(IPAddress& dest, const IPAddress_t& src)
    {
        IPAddress const* raw = (IPAddress const*)src.bytes;
        dest                 = *raw;
    }

}  // namespace ncore
#    endif

#    ifdef TARGET_ESP8266
#        include "IPAddress.h"

namespace ncore
{
    IPAddress_t IPAddress_t::from(u32 ip)
    {
        IPAddress_t addr;
        IPAddress   ipAddr(ip);
        IPAddress_t::from_arduino(addr, ipAddr);
        return addr;
    }

    IPAddress_t IPAddress_t::from(u8 a, u8 b, u8 c, u8 d)
    {
        IPAddress_t addr;
        IPAddress   ipAddr(a, b, c, d);
        IPAddress_t::from_arduino(addr, ipAddr);
        return addr;
    }

    void IPAddress_t::from_arduino(IPAddress_t& dest, const IPAddress& src)
    {
        dest.flags     = src.isV4() ? 4 : 6;
        IPAddress* raw = (IPAddress*)dest.bytes;
        *raw           = src;
    }

    void IPAddress_t::to_arduino(IPAddress& dest, const IPAddress_t& src)
    {
        IPAddress const* raw = (IPAddress const*)src.bytes;
        dest                 = *raw;
    }
}  // namespace ncore

#    endif

#endif

namespace ncore
{
    // examples of valid IP string addresses:
    // "10.0.0.1"
    // "192.168.4.1"
    // examples of invalid IP string addresses:
    // "256.0.0.1"   (256 is out of range)
    // "192.168.4"   (only 3 parts)
    // "10.001.1.1   (leading zero)

    bool IPAddress_t::from_string(str_t const& str)
    {
        s32 partIndex = 0;
        u32 partValue = 0;
        s32 partChars = 0;

        u8 ip[16] = {0};

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
                ip[partIndex] = (partValue << ((3 - partIndex) * 8));
                partValue     = 0;
                partChars     = 0;
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

#ifdef TARGET_ESP32
        if (partIndex == 4)
        {
            IPAddress ip(ip[0], ip[1], ip[2], ip[3]);
            IPAddress_t::from_arduino(*this, ip);
            return true;
        }
        else if (partIndex == 16)
        {
            IPAddress ip(ip[0], ip[1], ip[2], ip[3], ip[4], ip[5], ip[6], ip[7], ip[8], ip[9], ip[10], ip[11], ip[12], ip[13], ip[14], ip[15]);
            IPAddress_t::from_arduino(*this, ip);
            return true;
        }
        reset();
        return false;  // Not enough or too many parts
#endif
#ifdef TARGET_ESP8266
        if (partIndex == 4)
        {
            IPAddress ip(ip[0], ip[1], ip[2], ip[3]);
            IPAddress_t::from_arduino(*this, ip);
            return true;
        }
        reset();
#endif
        return false;  // Not enough or too many parts
    }

    void IPAddress_t::to_string(str_t& str) const
    {
        if (flags == 6)
        {
            // IPv6 not implemented yet
            str_append(str, "[IPv6 address]");
            return;
        }

#ifdef TARGET_ARDUINO
        const IPAddress& ip  = *(IPAddress const*)bytes;
        const char*      sep = ".";
        to_str(str, (byte)ip[0], 0, 10);
        str_append(str, sep);
        to_str(str, (byte)(ip[1]) & 0xFF, 0, 10);
        str_append(str, sep);
        to_str(str, (byte)(ip[2]) & 0xFF, 0, 10);
        str_append(str, sep);
        to_str(str, (byte)(ip[3]) & 0xFF, 0, 10);
#endif
    }

    bool IPAddress_t::is_valid() const
    {
#ifdef TARGET_ARDUINO
        const IPAddress& ip = *(IPAddress const*)bytes;
        // Exclude special ranges: 0.x.x.x, 127.x.x.x, 224.x.x.x - 255.x.x.x
        if (flags == 4)
        {
            return ip[0] != 0 && ip[0] != 127 && (ip[0] < 224);
        }
        else if (flags == 6)
        {
            // For IPv6, we can add more checks if needed
            return true;  // Accept all IPv6 addresses for now
        }
#endif
        return false;
    }

}  // namespace ncore