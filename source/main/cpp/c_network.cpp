#include "rdno_core/c_network.h"

namespace ncore
{
    // examples of valid IP string addresses:
    // "10.0.0.1"
    // "192.168.4.1"
    // examples of invalid IP string addresses:
    // "256.0.0.1"   (256 is out of range)
    // "192.168.4"   (only 3 parts)
    // "10.001.1.1   (leading zero)

    bool FromString(const char* str, s32 strLen, IPAddress_t& outAddr)
    {
        if (str == nullptr)
            return false;

        byte parts[4]  = {0, 0, 0, 0};
        s32  partIndex = 0;
        s32  partValue = 0;
        s32  partChars = 0;

        const char* end = str + strLen;
        while (str <= end)
        {
            const char ch = str < end ? *str : '.';
            if (ch >= '0' && ch <= '9')
            {
                partValue = partValue * 10 + (ch - '0');
                partChars++;
            }
            else if (ch == '.')
            {
                if (partChars == 0 || partIndex >= 4)
                    return false;  // Invalid format or too many parts
                if ((partValue <= 9 && partChars != 1) || (partValue > 9 && partValue <= 99 && partChars != 2))
                    return false;  // Leading zero is not allowed
                if (partValue > 255)
                    return false;  // Each part must be in the range 0-255
                parts[partIndex++] = (byte)partValue;
                partValue          = 0;
                partChars          = 0;
            }
            else
            {
                return false;  // Invalid character
            }
            str++;
        }

        if (partIndex == 4)
        {
            outAddr.m_address[0] = parts[0];
            outAddr.m_address[1] = parts[1];
            outAddr.m_address[2] = parts[2];
            outAddr.m_address[3] = parts[3];
            return true;
        }

        return false;  // Invalid format
    }

    bool FromString(const char* str, s32 strLen, MACAddress_t& outAddr)
    {
        if (str == nullptr)
            return false;

        byte parts[6]  = {0, 0, 0, 0, 0, 0};
        s32  partIndex = 0;
        s32  partValue = 0;
        s32  partChars = 0;

        const char* end = str + strLen;
        while (str <= end)
        {
            const char ch = str < end ? *str : ':';
            if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
            {
                partValue = (partValue << 4) | (ch >= '0' && ch <= '9' ? (ch - '0') : (ch >= 'a' && ch <= 'f' ? (ch - 'a' + 10) : (ch - 'A' + 10)));
                partChars++;
            }
            else if (ch == ':')
            {
                if (partChars == 0 || partIndex >= 6)
                    return false;  // Invalid format or too many parts
                if (partChars == 0 || partChars > 2)
                    return false;  // Each part must have 1 or 2 hex digits
                if (partValue > 0xFF)
                    return false;  // Each part must be in the range 0x00-0
                parts[partIndex++] = partValue;
                partValue          = 0;
                partChars          = 0;
            }
            else
            {
                return false;  // Invalid character
            }

            str++;
        }

        if (partIndex == 6)
        {
            outAddr.m_address[0] = parts[0];
            outAddr.m_address[1] = parts[1];
            outAddr.m_address[2] = parts[2];
            outAddr.m_address[3] = parts[3];
            outAddr.m_address[4] = parts[4];
            outAddr.m_address[5] = parts[5];
            return true;
        }

        return false;  // Invalid format
    }

    bool IsValidSSID(const char* ssid, s32 strLen)
    {
        if (ssid == nullptr)
            return false;
        const char* iter = ssid;
        const char* end  = ssid + strLen;
        while (iter < end)
        {
            char ch = *iter;
            if (ch < 32 || ch > 126)  // Printable ASCII range
                return false;
            iter++;
        }
        const s32 length = (s32)(iter - ssid);
        return length > 0 && length <= 32;
    }

    bool IsValidPassword(const char* password, s32 strLen)
    {
        if (password == nullptr)
            return false;
        const char* iter = password;
        const char* end  = password + strLen;
        while (iter < end)
        {
            char ch = *iter;
            if (ch < 32 || ch > 126)  // Printable ASCII range
                return false;
            iter++;
        }
        const s32 length = (s32)(iter - password);
        return length >= 8 && length <= 64;
    }

    bool IsValidIPAddress(const char* ipAddress, s32 strLen)
    {
        IPAddress_t addr;
        return FromString(ipAddress, strLen, addr);
    }

}  // namespace ncore
