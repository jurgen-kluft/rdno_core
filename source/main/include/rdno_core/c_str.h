#ifndef __RDNO_CORE_STR_H__
#define __RDNO_CORE_STR_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    // -------------------------------------------------------------------------------
    // character functions
    // '0' = 0x30, '9' = 0x39, 'A' = 0x41, 'Z' = 0x5A, 'a' = 0x61, 'z' = 0x7A
    inline bool is_space(char c) { return ((c == 0x09) || (c == 0x0A) || (c == 0x0D) || (c == ' ')); }
    inline bool is_whitespace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }
    inline bool is_upper(char c) { return ((c >= 'A') && (c <= 'Z')); }
    inline bool is_lower(char c) { return ((c >= 'a') && (c <= 'z')); }
    inline bool is_alpha(char c) { return (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }
    inline bool is_digit(char c) { return ((c >= '0') && (c <= '9')); }
    inline bool is_hexa(char c) { return (((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || ((c >= '0') && (c <= '9'))); }
    inline char to_upper(char c) { return ((c >= 'a') && (c <= 'z')) ? c + (char)('A' - 'a') : c; }
    inline char to_lower(char c) { return ((c >= 'A') && (c <= 'Z')) ? c + (char)('a' - 'A') : c; }
    inline char to_dec_char(u8 val) { return "0123456789??????"[val & 0xf]; }
    inline char to_hex_char(u8 val, bool lowercase) { return (lowercase) ? "0123456789abcdef"[val & 0xf] : "0123456789ABCDEF"[val & 0xf]; }
    inline byte from_char(char c) { return is_digit(c) ? (c - '0') : ((c >= 'A') && (c <= 'F')) ? (c - 'A' + 10) : ((c >= 'a') && (c <= 'f')) ? (c - 'a' + 10) : 255; }

    // string type ----------------------------------------------------------------------------
    // The string is defined by the range [m_str, m_end) within the buffer [0, m_eos).
    // The buffer must always be null-terminated at m_eos, but the string itself
    // does not need to be null-terminated.
    // The string can be empty (m_str == m_end) but must always satisfy:
    //   0 <= m_str <= m_end <= m_eos <= 32767
    // The buffer can be mutable (m_ascii != nullptr) or read-only (m_ascii == nullptr).
    // If the buffer is mutable, then the string can be modified in-place.
    struct str_t
    {
        const char* m_const;  // pointer to a read-only ascii string
        char*       m_ascii;  // pointer to a mutable ascii string
        s16         m_str;    // m_const[m_str] is the first character of the string
        s16         m_end;    // m_const[m_end] is one past the last character
        s16         m_eos;    // m_const[m_eos] is the end of the string but always points to a terminator
    };

    // string properties ----------------------------------------------------------------------
    inline s16  str_len(const str_t& s) { return s.m_end - s.m_str; }
    inline char str_at(const str_t& s, s16 index)
    {
        index += s.m_str;
        return index < s.m_end ? s.m_const[index] : '\0';
    }
    inline char*       str_write_ptr(const str_t& s) { return s.m_ascii + s.m_str; }
    inline const char* str_const_ptr(const str_t& s) { return s.m_const + s.m_str; }
    inline const char* str_const_end(const str_t& s) { return s.m_const + s.m_end; }
    inline bool        str_is_empty(const str_t& s) { return s.m_str == s.m_end; }

    // constructors ----------------------------------------------------------------------------
    str_t str_empty();                                                 // create an empty str
    void  str_clear(str_t& ps);                                        // clear the string to empty
    str_t str_mutable(char* s, s16 capacity);                          // create a str to the given buffer
    str_t str_const(const char* s);                                    // create a str to the given null-terminated string
    str_t str_const_n(const char* s, s16 len);                         // create a str to the given null-terminated string
    str_t str_const_full(const char* s, s16 begin, s16 end, s16 eos);  // create a reference to the given range of chars

    // selections -----------------------------------------------------------------------------
    str_t str_select_after(const str_t& str, const str_t& sel);   // select the part after the incoming string
    str_t str_select_before(const str_t& str, const str_t& sel);  // select the part before the incoming string
    str_t str_select_until(const str_t& str, const str_t& sel);   // widen 'str' until the beginning of 'part'

    // string movements -----------------------------------------------------------------------
    void str_swap(str_t& s1, str_t& s2);  // swap two string objects

    // string comparisons and search -----------------------------------------------------------
    s32         str_cmp(const str_t& s1, const str_t& s2, bool case_sensitive = true);  // compare two strings lexicographically
    inline bool str_eq(const str_t& s1, const str_t& s2, bool case_sensitive = true) { return str_cmp(s1, s2, case_sensitive) == 0; }
    bool        str_has_prefix(const str_t& s, const str_t& prefix, bool case_sensitive = true);  // test for prefix
    bool        str_has_suffix(const str_t& s, const str_t& suffix, bool case_sensitive = true);  // test for suffix
    bool        str_contains(const str_t& s, const str_t& substr, bool case_sensitive = true);    // test for substring
    str_t       str_find(const str_t& s, const str_t& substr, bool case_sensitive = true);        // find first occurrence of substring
    str_t       str_find_last(const str_t& s, const str_t& substr, bool case_sensitive = true);   // find last occurrence of substring
    str_t       str_find_one_of(const str_t& s, const str_t& chars, bool case_sensitive = true);  // find first occurrence of any of the given characters

    s32         str_cmp(const str_t& s1, const char* s2, bool case_sensitive = true);               // compare two strings lexicographically
    s32         str_cmp_n(const str_t& s1, const char* s2, s32 s2Len, bool case_sensitive = true);  // compare two strings lexicographically
    inline bool str_eq(const str_t& s1, const char* s2, bool case_sensitive = true) { return str_cmp(s1, s2, case_sensitive) == 0; }
    inline bool str_eq_n(const str_t& s1, const char* s2, s32 s2Len, bool case_sensitive = true) { return str_cmp_n(s1, s2, s2Len, case_sensitive) == 0; }
    bool        str_has_prefix(const str_t& s, const char* prefix, bool case_sensitive = true);  // test for prefix
    bool        str_has_suffix(const str_t& s, const char* suffix, bool case_sensitive = true);  // test for suffix
    bool        str_contains(const str_t& s, const char* substr, bool case_sensitive = true);    // test for substring
    str_t       str_find(const str_t& s, const char* substr, bool case_sensitive = true);        // find first occurrence of substring
    str_t       str_find_last(const str_t& s, const char* substr, bool case_sensitive = true);   // find last occurrence of substring
    str_t       str_find_one_of(const str_t& s, const char* chars, bool case_sensitive = true);  // find first occurrence of any of the given characters

    bool  str_contains(const str_t& s, char c, bool case_sensitive = true);   // test for character
    str_t str_find(const str_t& s, char c, bool case_sensitive = true);       // find first occurrence of c
    str_t str_find_last(const str_t& s, char c, bool case_sensitive = true);  // find last occurrence of c

    // string trimming ----------------------------------------------------------------------
    str_t str_trim_left(const str_t& s);                                        // trim whitespace from the left
    str_t str_trim_right(const str_t& s);                                       // trim whitespace from the right
    str_t str_trim(const str_t& s);                                             // trim whitespace from both ends
    str_t str_trimQuotes(const str_t& s);                                       // trim quotes from both ends if present
    str_t str_trimQuotes(const str_t& s, char quoteChar);                       // trim specified quote char from both ends if present
    str_t str_trimDelimiters(const str_t& s, char leftDelim, char rightDelim);  // trim specified delimiters from both ends if present

    // from and to conversions ----------------------------------------------------------------
    bool from_str(const str_t& s, bool* outValue);
    bool from_str(const str_t& s, s32* outValue, s16 base = 10);
    bool from_str(const str_t& s, u32* outValue, s16 base = 10);
    bool from_str(const str_t& s, s64* outValue, s16 base = 10);
    bool from_str(const str_t& s, u64* outValue, s16 base = 10);
    bool from_str(const str_t& s, f32* outValue);

    void to_str(str_t& dest, bool value);
    void to_str(str_t& dest, byte value, s8 chars = 0, s16 base = 10);
    void to_str(str_t& dest, s32 value, s16 base = 10);
    void to_str(str_t& dest, u32 value, s16 base = 10);
    void to_str(str_t& dest, f32 value, s16 num_fractional_digits = 2);

    // string composition ------------------------------------------------------------------
    s16 str_append(str_t& dest, char c);          // append single character to 'dest' (note: dest must be mutable)
    s16 str_append(str_t& dest, const str_t& s);  // append 's' to 'dest' (note: dest must be mutable)
    s16 str_append(str_t& dest, const char* src);
    s16 str_append(str_t& dest, const str_t* array, s16 count);

    // implementation helpers
    s16 str_join(str_t& dest, char sep, const str_t& src1, const str_t& src2);
    s16 str_join(str_t& dest, char sep, const str_t& src1, const str_t& src2, const str_t& src3);
    s16 str_join(str_t& dest, char sep, const str_t* src_array, s16 array_count);
    s16 str_join(str_t& dest, const str_t& sep, const str_t* src_array, s16 array_count);

    // printing to serial -------------------------------------------------------------------
    void str_print(const str_t& s);
    void str_println(const str_t& s);

}  // namespace ncore

#endif  // __RDNO_CORE_STR_H__