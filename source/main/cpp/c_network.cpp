#include "rdno_core/c_network.h"

#include "Arduino.h"

namespace ncore
{
    // examples of valid IP string addresses:
    // "10.0.0.1"
    // "192.168.4.1"
    // examples of invalid IP string addresses:
    // "256.0.0.1"   (256 is out of range)
    // "192.168.4"   (only 3 parts)
    // "10.001.1.1   (leading zero)

    bool FromString(const char* str, IPAddress_t& outAddr)
    {
        if (str == nullptr)
            return false;

        s32  parts[4]    = {0, 0, 0, 0};
        s32  partIndex   = 0;
        s32  partValue   = 0;
        bool parsingPart = false;

        while (*str != '\0')
        {
            char ch = *str;
            if (ch >= '0' && ch <= '9')
            {
                if (!parsingPart && partValue == 0 && ch == '0')
                    return false;  // Leading zero is not allowed
                partValue = partValue * 10 + (ch - '0');
                if (partValue > 255)
                    return false;  // Each part must be in the range 0-255
                parsingPart = true;
            }
            else if (ch == '.')
            {
                if (!parsingPart || partIndex >= 3)
                    return false;  // Invalid format or too many parts
                parts[partIndex++] = partValue;
                partValue          = 0;
                parsingPart        = false;
            }
            else
            {
                return false;  // Invalid character
            }
            str++;
        }

        if (parsingPart && partIndex == 3)
        {
            parts[partIndex]     = partValue;  // Last part
            outAddr.m_address[0] = static_cast<byte>(parts[0]);
            outAddr.m_address[1] = static_cast<byte>(parts[1]);
            outAddr.m_address[2] = static_cast<byte>(parts[2]);
            outAddr.m_address[3] = static_cast<byte>(parts[3]);
            return true;
        }

        return false;  // Invalid format
    }

    bool FromString(const char* str, MACAddress_t& outAddr)
    {
        if (str == nullptr)
            return false;

        s32  parts[6]    = {0, 0, 0, 0, 0, 0};
        s32  partIndex   = 0;
        s32  partValue   = 0;
        bool parsingPart = false;

        while (*str != '\0')
        {
            char ch = *str;
            if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
            {
                parsingPart = true;
                partValue   = (partValue << 4) | (ch >= '0' && ch <= '9' ? (ch - '0') : (ch >= 'a' && ch <= 'f' ? (ch - 'a' + 10) : (ch - 'A' + 10)));
                if (partValue > 255)
                    return false;  // Each part must be in the range 0-255
            }
            else if (ch == ':')
            {
                if (!parsingPart || partIndex >= 5)
                    return false;  // Invalid format or too many parts
                parts[partIndex++] = partValue;
                partValue          = 0;
                parsingPart        = false;
            }
            else
            {
                return false;  // Invalid character
            }
            str++;
        }

        if (parsingPart && partIndex == 5)
        {
            parts[partIndex]     = partValue;  // Last part
            outAddr.m_address[0] = static_cast<byte>(parts[0]);
            outAddr.m_address[1] = static_cast<byte>(parts[1]);
            outAddr.m_address[2] = static_cast<byte>(parts[2]);
            outAddr.m_address[3] = static_cast<byte>(parts[3]);
            outAddr.m_address[4] = static_cast<byte>(parts[4]);
            outAddr.m_address[5] = static_cast<byte>(parts[5]);
            return true;
        }

        return false;  // Invalid format
    }

    bool IsValidSSID(const char* ssid)
    {
        if (ssid == nullptr)
            return false;
        s32 length = strlen(ssid);
        return length > 0 && length <= 32;
    }

    bool IsValidPassword(const char* password)
    {
        if (password == nullptr)
            return false;
        s32 length = strlen(password);
        return length >= 8 && length <= 64;
    }

    bool IsValidIPAddress(const char* ipAddress)
    {
        IPAddress_t addr;
        return FromString(ipAddress, addr);
    }

}  // namespace ncore
