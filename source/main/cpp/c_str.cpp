#include "rdno_core/c_str.h"
#include "rdno_core/c_serial.h"
#include "ccore/c_memory.h"

namespace ncore
{
    static const str_t s_empty = {"", nullptr, 0, 0, 0};

    inline s16 str_len(const char* str)
    {
        if (str == nullptr)
            return 0;
        s16 len = 0;
        while (str[len] != '\0' && len < 0x7fff)
            len++;
        return len;
    }

    str_t str_empty() { return s_empty; }

    void str_clear(str_t& ps)
    {
        if (ps.m_ascii != nullptr)
        {
            ps.m_ascii[0] = '\0';
            ps.m_const    = ps.m_ascii;
            ps.m_str      = 0;
            ps.m_end      = 0;
        }
        else
        {
            ps.m_ascii = nullptr;
            ps.m_const = "";
            ps.m_str   = 0;
            ps.m_end   = 0;
            ps.m_eos   = 0;
        }
    }

    str_t str_mutable(char* s, s16 capacity)
    {
        str_t ps;
        ps.m_const = s;
        ps.m_ascii = s;
        ps.m_str   = 0;
        ps.m_end   = 0;
        ps.m_eos   = (capacity > 0) ? capacity - 1 : 0;
        if (s != nullptr)
        {
            s[ps.m_end] = '\0';
            s[ps.m_eos] = '\0';
        }
        return ps;
    }

    str_t str_const(const char* s)
    {
        const s16 len = str_len(s);
        return str_const_n(s, len);
    }

    str_t str_const_n(const char* s, s16 len)
    {
        str_t ps;
        ps.m_const = (s != nullptr) ? s : "";
        ps.m_ascii = nullptr;
        ps.m_str   = 0;
        ps.m_end   = len;
        ps.m_eos   = len;
        return ps;
    }

    str_t str_const_full(const char* s, s16 begin, s16 end, s16 eos)
    {
        str_t ps;
        ps.m_const = (s != nullptr) ? s : "";
        ps.m_ascii = nullptr;
        ps.m_str   = begin;
        ps.m_end   = end;
        ps.m_eos   = eos;
        return ps;
    }

    str_t str_select_after(const str_t& str, const str_t& sel)
    {
        str_t ps;
        ps.m_ascii = str.m_ascii;
        ps.m_const = str.m_const;
        ps.m_str   = sel.m_end;
        ps.m_end   = str.m_end;
        ps.m_eos   = str.m_eos;
        return ps;
    }

    str_t str_select_before(const str_t& str, const str_t& sel)
    {
        str_t ps;
        ps.m_ascii = str.m_ascii;
        ps.m_const = str.m_const;
        ps.m_str   = str.m_str;
        ps.m_end   = sel.m_str;
        ps.m_eos   = str.m_eos;
        return ps;
    }

    str_t str_select_until(const str_t& str, const str_t& sel)
    {
        str_t ps;
        ps.m_ascii = str.m_ascii;
        ps.m_const = str.m_const;
        ps.m_str   = str.m_str;
        ps.m_end   = sel.m_str;
        ps.m_eos   = str.m_eos;
        return ps;
    }

    void str_swap(str_t& s1, str_t& s2)
    {
        str_t temp = s1;
        s1         = s2;
        s2         = temp;
    }

    s32 str_cmp(const str_t& s1, const str_t& s2, bool case_sensitive)
    {
        s16 len1 = str_len(s1);
        s16 len2 = str_len(s2);
        s16 len  = (len1 < len2) ? len1 : len2;

        for (s16 i = 0; i < len; i++)
        {
            char c1 = s1.m_const[s1.m_str + i];
            char c2 = s2.m_const[s2.m_str + i];
            if (!case_sensitive)
            {
                c1 = to_lower(c1);
                c2 = to_lower(c2);
            }
            if (c1 != c2)
                return (c1 < c2) ? -1 : 1;
        }
        if (len1 == len2)
            return 0;
        return (len1 < len2) ? -1 : 1;
    }

    bool str_has_prefix(const str_t& s, const str_t& prefix, bool case_sensitive)
    {
        s16 lenS      = str_len(s);
        s16 lenPrefix = str_len(prefix);
        if (lenPrefix > lenS)
            return false;
        for (s16 i = 0; i < lenPrefix; i++)
        {
            char c1 = s.m_const[s.m_str + i];
            char c2 = prefix.m_const[prefix.m_str + i];
            if (!case_sensitive)
            {
                c1 = to_lower(c1);
                c2 = to_lower(c2);
            }
            if (c1 != c2)
                return false;
        }
        return true;
    }

    bool str_has_suffix(const str_t& s, const str_t& suffix, bool case_sensitive)
    {
        s16 lenS      = str_len(s);
        s16 lenSuffix = str_len(suffix);
        if (lenSuffix > lenS)
            return false;
        for (s16 i = 0; i < lenSuffix; i++)
        {
            char c1 = s.m_const[s.m_end - lenSuffix + i];
            char c2 = suffix.m_const[suffix.m_str + i];
            if (!case_sensitive)
            {
                c1 = to_lower(c1);
                c2 = to_lower(c2);
            }
            if (c1 != c2)
                return false;
        }
        return true;
    }

    bool str_contains(const str_t& s, const str_t& substr, bool case_sensitive)
    {
        str_t found = str_find(s, substr, case_sensitive);
        return !str_is_empty(found);
    }

    str_t str_find(const str_t& s, const str_t& substr, bool case_sensitive)
    {
        s16 lenS      = str_len(s);
        s16 lenSubstr = str_len(substr);
        if (lenSubstr == 0 || lenSubstr > lenS)
            return s_empty;  // empty substring or substring longer than string

        for (s16 i = 0; i <= lenS - lenSubstr; i++)
        {
            bool match = true;
            for (s16 j = 0; j < lenSubstr && match; j++)
            {
                char c1 = s.m_const[s.m_str + i + j];
                char c2 = substr.m_const[substr.m_str + j];
                if (!case_sensitive)
                {
                    c1 = to_lower(c1);
                    c2 = to_lower(c2);
                }
                match = (c1 == c2);
            }
            if (match)
            {
                str_t found;
                found.m_ascii = s.m_ascii;
                found.m_const = s.m_const;
                found.m_str   = s.m_str + i;
                found.m_end   = s.m_str + i + lenSubstr;
                found.m_eos   = s.m_eos;
                return found;
            }
        }
        return s_empty;  // not found
    }

    str_t str_find_last(const str_t& s, const str_t& substr, bool case_sensitive)
    {
        s16 lenS      = str_len(s);
        s16 lenSubstr = str_len(substr);
        if (lenSubstr == 0 || lenSubstr > lenS)
            return s_empty;  // empty substring or substring longer than string

        for (s16 i = lenS - lenSubstr; i >= 0; i--)
        {
            bool match = true;
            for (s32 j = 0; j < lenSubstr && match; j++)
            {
                char c1 = s.m_const[s.m_str + i + j];
                char c2 = substr.m_const[substr.m_str + j];
                if (!case_sensitive)
                {
                    c1 = to_lower(c1);
                    c2 = to_lower(c2);
                }
                match = (c1 == c2);
            }
            if (match)
            {
                str_t found;
                found.m_ascii = s.m_ascii;
                found.m_const = s.m_const;
                found.m_str   = s.m_str + i;
                found.m_end   = s.m_str + i + lenSubstr;
                found.m_eos   = s.m_eos;
                return found;
            }
        }
        return s_empty;  // not found
    }

    str_t str_find_one_of(const str_t& s, const str_t& chars, bool case_sensitive)
    {
        s16 lenS     = str_len(s);
        s16 lenChars = str_len(chars);
        if (lenChars == 0 || lenS == 0)
            return s_empty;  // empty chars or string

        for (s16 i = 0; i < lenS; i++)
        {
            const char c1 = s.m_const[s.m_str + i];
            if (str_contains(chars, c1, case_sensitive))
            {
                str_t found;
                found.m_ascii = s.m_ascii;
                found.m_const = s.m_const;
                found.m_str   = s.m_str + i;
                found.m_end   = s.m_str + i + 1;
                found.m_eos   = s.m_eos;
                return found;
            }
        }
        return s_empty;  // not found
    }

    s32 str_cmp(const str_t& s1, const char* s2, bool case_sensitive)
    {
        str_t ps2 = str_const(s2);
        return str_cmp(s1, ps2, case_sensitive);
    }

    s32 str_cmp_n(const str_t& s1, const char* s2, s32 s2Len, bool case_sensitive)
    {
        str_t ps2 = str_const_n(s2, (s16)s2Len);
        return str_cmp(s1, ps2, case_sensitive);
    }

    bool str_has_prefix(const str_t& s1, const char* prefix, bool case_sensitive)
    {
        str_t sp = str_const(prefix);
        return str_has_prefix(s1, sp, case_sensitive);
    }

    bool str_has_suffix(const str_t& s, const char* suffix, bool case_sensitive)
    {
        str_t ss = str_const(suffix);
        return str_has_suffix(s, ss, case_sensitive);
    }

    bool str_contains(const str_t& s, const char* substr, bool case_sensitive)
    {
        str_t ss = str_const(substr);
        return str_contains(s, ss, case_sensitive);
    }

    str_t str_find(const str_t& s, const char* substr, bool case_sensitive)
    {
        str_t ss = str_const(substr);
        return str_find(s, ss, case_sensitive);
    }

    str_t str_find_last(const str_t& s, const char* substr, bool case_sensitive)
    {
        str_t ss = str_const(substr);
        return str_find_last(s, ss, case_sensitive);
    }

    str_t str_find_one_of(const str_t& s, const char* chars, bool case_sensitive)
    {
        str_t sc = str_const(chars);
        return str_find_one_of(s, sc, case_sensitive);
    }

    s16 str_find_index(const str_t& s, char c, bool case_sensitive)
    {
        s16 index = s.m_str;
        while (index < s.m_end)
        {
            char ch = s.m_const[index];
            if (!case_sensitive)
                ch = to_lower(ch);
            if (ch == c)
                return index;
            index++;
        }
        return -1;
    }

    bool str_contains(const str_t& s, char c, bool case_sensitive) { return str_find_index(s, c, case_sensitive) != -1; }

    str_t str_find(const str_t& s, char c, bool case_sensitive)
    {
        const s16 index = str_find_index(s, c, case_sensitive);

        str_t found;
        found.m_ascii = s.m_ascii;
        found.m_const = s.m_const;
        found.m_eos   = s.m_eos;

        if (index == -1)
        {
            found.m_str = s.m_end;
            found.m_end = s.m_end;
            return found;  // not found
        }

        found.m_str = index;
        found.m_end = index + 1;
        return found;
    }

    str_t str_find_last(const str_t& s, char c, bool case_sensitive)
    {
        const char* ptr   = s.m_const + s.m_end - 1;
        const char* start = s.m_const + s.m_str;
        while (ptr >= start)
        {
            char ch = *ptr;
            if (!case_sensitive)
                ch = to_lower(ch);
            if (ch == c)
            {
                str_t found;
                found.m_ascii = s.m_ascii;
                found.m_const = s.m_const;
                found.m_str   = (s16)(ptr - s.m_const);
                found.m_end   = s.m_end;
                found.m_eos   = s.m_eos;
                return found;
            }
            ptr--;
        }
        return s_empty;  // not found
    }

    bool from_str(const str_t& s, bool* outValue)
    {
        if (str_has_prefix(s, "true", false))
        {
            *outValue = true;
            return true;
        }
        else if (str_has_prefix(s, "false", false))
        {
            *outValue = false;
            return true;
        }
        return false;
    }

    bool str_to_int32(const char* str, const char* end, s16 base, s32* outValue)
    {
        *outValue = 0;

        if (str == nullptr || str == end || base < 2 || base > 36)
            return false;

        bool        isNegative = false;
        s32         value      = 0;
        const char* ptr        = str;

        // Handle optional sign
        if (ptr < end)
        {
            if (*ptr == '-')
            {
                isNegative = true;
                ptr++;
            }
            else if (*ptr == '+')
            {
                ptr++;
            }

            // If base == 16, we can have an optional "0x" or "0X" prefix
            if (base == 16 && (end - ptr) >= 2 && ptr[0] == '0' && (ptr[1] == 'x' || ptr[1] == 'X'))
            {
                ptr += 2;
            }

            // Convert characters to integer
            while (ptr < end)
            {
                const char c     = *ptr;
                const s32  digit = from_char(c);
                if (digit >= base)
                    return false;
                value = value * base + digit;
                ptr++;
            }

            *outValue = isNegative ? -value : value;
            return true;
        }
        return false;
    }

    bool str_to_int64(const char* str, const char* end, s16 base, s64* outValue)
    {
        *outValue = 0;

        if (str == nullptr || str == end || base < 2 || base > 36)
            return false;

        bool        isNegative = false;
        s32         value      = 0;
        const char* ptr        = str;

        // Handle optional sign
        if (ptr < end)
        {
            if (*ptr == '-')
            {
                isNegative = true;
                ptr++;
            }
            else if (*ptr == '+')
            {
                ptr++;
            }

            // If base == 16, we can have an optional "0x" or "0X" prefix
            if (base == 16 && (end - ptr) >= 2 && ptr[0] == '0' && (ptr[1] == 'x' || ptr[1] == 'X'))
            {
                ptr += 2;
            }

            // Convert characters to integer
            while (ptr < end)
            {
                const char c     = *ptr;
                const s32  digit = from_char(c);
                if (digit >= base)
                    return false;
                value = value * base + digit;
                ptr++;
            }

            *outValue = isNegative ? -value : value;
            return true;
        }
        return false;
    }

    bool str_to_float(const char* str, const char* end, f32* outValue)
    {
        // This will only be able to handle simple float representations like "123.456" or "-78.9"
        // It does not handle scientific notation or other complex formats.
        *outValue = 0.0f;

        if (str == nullptr || str == end)
            return false;

        bool        isNegative = false;
        f32         value      = 0.0f;
        f32         fraction   = 0.0f;
        f32         divisor    = 1.0f;
        bool        inFraction = false;
        const char* ptr        = str;

        if (ptr < end)
        {
            // Handle optional + and - signs
            if (*ptr == '-')
            {
                isNegative = true;
                ptr++;
            }
            else if (*ptr == '+')
            {
                ptr++;
            }

            // Convert characters to float
            while (ptr < end)
            {
                const char c = *ptr;
                if (c == '.')
                {
                    if (inFraction)
                        return false;  // multiple decimal points
                    inFraction = true;
                    ptr++;
                    continue;
                }

                const s32 digit = from_char(c);
                if (digit < 0 || digit > 9)
                    return false;  // invalid digit
                if (inFraction)
                {
                    divisor *= 10.0f;
                    fraction += digit / divisor;
                }
                else
                {
                    value = value * 10.0f + digit;
                }
                ptr++;
            }
            value += fraction;
            if (isNegative)
                value = -value;
            *outValue = value;
            return true;
        }
        return false;
    }

    bool from_str(const str_t& s, s32* outValue, s16 base)
    {
        if (outValue == nullptr)
            return false;

        str_t trimmed = str_trim(s);
        if (str_is_empty(trimmed))
            return false;

        const char* start = trimmed.m_const + trimmed.m_str;
        const char* end   = trimmed.m_const + trimmed.m_end;
        return str_to_int32(start, end, base, outValue);
    }

    bool from_str(const str_t& s, u32* outValue, s16 base)
    {
        if (outValue == nullptr)
            return false;

        s32  intValue = 0;
        bool success  = from_str(s, &intValue, base);
        if (success && intValue >= 0)
        {
            *outValue = (u32)intValue;
            return true;
        }
        return false;
    }

    bool from_str(const str_t& s, s64* outValue, s16 base)
    {
        if (outValue == nullptr)
            return false;

        str_t trimmed = str_trim(s);
        if (str_is_empty(trimmed))
            return false;

        const char* start = trimmed.m_const + trimmed.m_str;
        const char* end   = trimmed.m_const + trimmed.m_end;
        return str_to_int64(start, end, base, outValue);
    }

    bool from_str(const str_t& s, u64* outValue, s16 base)
    {
        if (outValue == nullptr)
            return false;

        s64  intValue = 0;
        bool success  = from_str(s, &intValue, base);
        if (success && intValue >= 0)
        {
            *outValue = (u32)intValue;
            return true;
        }
        return false;
    }

    bool from_str(const str_t& s, f32* outValue)
    {
        if (outValue == nullptr)
            return false;

        str_t trimmed = str_trim(s);
        if (str_is_empty(trimmed))
            return false;

        const char* start = trimmed.m_const + trimmed.m_str;
        const char* end   = trimmed.m_const + trimmed.m_end;
        return str_to_float(start, end, outValue);
    }

    void to_str(str_t& dest, bool value)
    {
        if (dest.m_ascii == nullptr)
            return;  // destination is not mutable

        const char* str = value ? "true" : "false";
        const s16   len = (value ? 4 : 5);

        s16 cursor = dest.m_end;
        for (s16 i = 0; i < len && cursor < dest.m_eos; i++)
            dest.m_ascii[cursor++] = str[i];

        dest.m_end               = cursor;
        dest.m_ascii[dest.m_end] = '\0';
        dest.m_const             = dest.m_ascii;
    }

    void to_str(str_t& dest, byte value, s8 chars, s16 base)
    {
        if (dest.m_ascii == nullptr || base < 2 || base > 36)
            return;  // destination is not mutable or invalid base

        if (dest.m_eos - dest.m_end < 2)
            return;

        if (base == 16 && chars == 0)
        {
            chars = 2;  // Default to 2 characters for hex if not specified
        }
        else if (base == 10 && chars == 0)
        {
            if (value >= 100)
                chars = 3;
            else if (value >= 10)
                chars = 2;
            else
                chars = 1;
        }

        const s16 begin = dest.m_end;
        s16       count = 0;
        do
        {
            const u32 digit            = value % base;
            dest.m_ascii[dest.m_end++] = to_hex_char((u8)digit, false);
            value /= base;
            count++;
        } while (value > 0 && dest.m_end < dest.m_eos && (chars == 0 || count < chars));

        // If a specific number of characters is requested, pad with leading zeros
        while (chars > 0 && count < chars && dest.m_end < dest.m_eos)
        {
            dest.m_ascii[dest.m_end++] = '0';
            count++;
        }
        // Reverse the string portion we just wrote
        s16 left  = begin;
        s16 right = dest.m_end - 1;
        while (left < right)
        {
            char temp           = dest.m_ascii[left];
            dest.m_ascii[left]  = dest.m_ascii[right];
            dest.m_ascii[right] = temp;
            left++;
            right--;
        }
        dest.m_ascii[dest.m_end] = '\0';
    }

    void to_str(str_t& dest, s32 value, s16 base)
    {
        if (dest.m_ascii == nullptr || base < 2 || base > 36)
            return;  // destination is not mutable or invalid base
        if (value < 0)
            dest.m_ascii[dest.m_end++] = '-';
        to_str(dest, (u32)((value < 0) ? -value : value), base);
    }

    void to_str(str_t& dest, u32 value, s16 base)
    {
        if (dest.m_ascii == nullptr || dest.m_end == dest.m_eos || base < 2 || base > 36)
            return;  // destination is not mutable or invalid base

        // Convert integer to string inplace
        const s16 begin  = dest.m_end;
        u32       uvalue = value;
        do
        {
            u32 digit                  = uvalue % base;
            dest.m_ascii[dest.m_end++] = to_hex_char((u8)digit, false);
            uvalue /= base;
        } while (uvalue > 0 && dest.m_end < dest.m_eos);

        // Reverse the string portion we just wrote
        s16 left  = begin;
        s16 right = dest.m_end - 1;
        while (left < right)
        {
            char temp           = dest.m_ascii[left];
            dest.m_ascii[left]  = dest.m_ascii[right];
            dest.m_ascii[right] = temp;
            left++;
            right--;
        }
        dest.m_ascii[dest.m_end] = '\0';
    }

    void to_str(str_t& dest, f32 value, s16 num_fractional_digits)
    {
        if (dest.m_ascii == nullptr || num_fractional_digits < 0 || num_fractional_digits > 6)
            return;  // destination is not mutable or invalid number of fractional digits

        // Handle negative values
        if (value < 0.0f)
        {
            dest.m_ascii[dest.m_end++] = '-';
            value                      = -value;
        }

        // Separate integer and fractional parts
        s32 intPart  = (s32)value;
        f32 fracPart = value - (f32)intPart;

        // Convert integer part
        to_str(dest, intPart, 10);

        // If no fractional digits are requested, we're done
        if (num_fractional_digits == 0)
            return;

        // Append decimal point
        dest.m_ascii[dest.m_end++] = '.';

        // Convert fractional part
        for (s16 i = 1; i <= num_fractional_digits; i++)
        {
            fracPart *= 10.0f;
            if (i == num_fractional_digits)
                fracPart += 0.5f;  // round last digit
            s32 digit = (s32)fracPart;
            fracPart -= (f32)digit;
            char c                     = to_dec_char((u8)digit);
            dest.m_ascii[dest.m_end++] = c;
        }
        dest.m_ascii[dest.m_end] = '\0';
    }

    void str_append(str_t& dest, char c)
    {
        if (dest.m_ascii == nullptr || dest.m_end >= dest.m_eos)
            return;  // destination is not mutable or no space left

        dest.m_ascii[dest.m_end++] = c;
        dest.m_ascii[dest.m_end]   = '\0';
    }

    void str_append(str_t& dest, const str_t& src)
    {
        if (dest.m_ascii == nullptr)
            return;  // destination is not mutable

        s16 len = str_len(src);
        if (len > (s16)(dest.m_eos - dest.m_end))
            len = (s16)(dest.m_eos - dest.m_end);  // truncate if necessary

        if (len > 0)
        {
            nmem::memcpy(&dest.m_ascii[dest.m_end], src.m_const + src.m_str, len);
            dest.m_end               = dest.m_end + len;
            dest.m_ascii[dest.m_end] = '\0';
            dest.m_const             = dest.m_ascii;
        }
    }

    void str_append(str_t& dest, const char* src)
    {
        if (dest.m_ascii == nullptr)
            return;  // destination is not mutable

        const char* ptr = src;
        while (*ptr != '\0' && dest.m_end < dest.m_eos)
            dest.m_ascii[dest.m_end++] = *ptr++;
        dest.m_ascii[dest.m_end] = '\0';
        dest.m_const             = dest.m_ascii;
    }

    void str_append(str_t& dest, const str_t* array, s16 count)
    {
        if (array == nullptr || count <= 0)
            return;

        for (s16 i = 0; i < count; i++)
            str_append(dest, array[i]);
    }

    void str_join(str_t& dest, char sep, const str_t& src1, const str_t& src2)
    {
        if (dest.m_ascii == nullptr)
            return;  // destination is not mutable

        str_append(dest, src1);
        str_append(dest, sep);
        str_append(dest, src2);
    }

    void str_join(str_t& dest, char sep, const str_t& src1, const str_t& src2, const str_t& src3)
    {
        if (dest.m_ascii == nullptr)
            return;  // destination is not mutable
        str_append(dest, src1);
        str_append(dest, sep);
        str_append(dest, src2);
        str_append(dest, sep);
        str_append(dest, src3);
    }

    void str_join(str_t& dest, char sep, const str_t* src_array, s16 array_count)
    {
        char  c[2]   = {sep, '\0'};
        str_t sepStr = str_const_n(c, 1);
        str_join(dest, sepStr, src_array, array_count);
    }

    void str_join(str_t& dest, const str_t& sep, const str_t* src_array, s16 array_count)
    {
        if (dest.m_ascii == nullptr || src_array == nullptr || array_count <= 0)
            return;  // destination is not mutable or invalid source array

        if (array_count > 0)
        {
            str_append(dest, src_array[0]);
            for (s16 i = 1; i < array_count; i++)
            {
                str_append(dest, sep);
                str_append(dest, src_array[i]);
            }
        }
    }

    str_t str_trim_left(const str_t& s)
    {
        str_t trimmed = s;
        while (trimmed.m_str < trimmed.m_end)
        {
            char c = trimmed.m_const[trimmed.m_str];
            if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
                break;
            trimmed.m_str++;
        }
        return trimmed;
    }

    str_t str_trim_right(const str_t& s)
    {
        str_t trimmed = s;
        while (trimmed.m_end > trimmed.m_str)
        {
            char c = trimmed.m_const[trimmed.m_end - 1];
            if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
                break;
            trimmed.m_end--;
        }
        return trimmed;
    }

    str_t str_trim(const str_t& s)
    {
        str_t trimmed = str_trim_left(s);
        trimmed       = str_trim_right(trimmed);
        return trimmed;
    }

    str_t str_trimQuotes(const str_t& s)
    {
        str_t trimmed = str_trimDelimiters(s, '"', '"');
        trimmed       = str_trimDelimiters(trimmed, '\'', '\'');
        return trimmed;
    }

    str_t str_trimQuotes(const str_t& s, char quoteChar) { return str_trimDelimiters(s, quoteChar, quoteChar); }

    str_t str_trimDelimiters(const str_t& s, char leftDelim, char rightDelim)
    {
        str_t trimmed = s;
        if (trimmed.m_end > trimmed.m_str + 1)
        {
            char firstChar = trimmed.m_const[trimmed.m_str];
            char lastChar  = trimmed.m_const[trimmed.m_end - 1];
            if (firstChar == leftDelim && lastChar == rightDelim)
            {
                trimmed.m_str++;
                trimmed.m_end--;
            }
        }
        return trimmed;
    }

    void str_print(const str_t& s)
    {
        if (s.m_ascii != nullptr && s.m_str < s.m_end)
        {
            const char e       = s.m_ascii[s.m_end];
            s.m_ascii[s.m_end] = '\0';

            nserial::print(&s.m_const[s.m_str]);

            s.m_ascii[s.m_end] = e;
        }
        else if (s.m_const != nullptr)
        {
            nserial::print(s.m_const);
        }
    }

    void str_println(const str_t& s)
    {
        if (s.m_ascii != nullptr)
        {
            if (s.m_str < s.m_end)
            {
                const char e       = s.m_ascii[s.m_end];
                s.m_ascii[s.m_end] = '\0';

                nserial::println(&s.m_const[s.m_str]);

                s.m_ascii[s.m_end] = e;
            }
        }
        else if (s.m_const != nullptr)
        {
            nserial::println(s.m_const);
        }
    }

}  // namespace ncore
