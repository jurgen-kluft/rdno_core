#ifndef __RDNO_CORE_STR_H__
#define __RDNO_CORE_STR_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    // reference: github, maxim2266/str

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
    inline u32  to_digit(char c) { return ((c >= '0') && (c <= '9')) ? (c - '0') : c; }
    inline u32  hex_to_number(char c) { return is_digit(c) ? (c - '0') : ((c >= 'A') && (c <= 'F')) ? (c - 'A' + 10) : ((c >= 'a') && (c <= 'f')) ? (c - 'a' + 10) : 0; }
    inline char to_dec_char(u8 val) { return "0123456789??????"[val & 0xf]; }
    inline char to_hex_char(u8 val, bool lowercase) { return (lowercase) ? "0123456789abcdef"[val & 0xf] : "0123456789ABCDEF"[val & 0xf]; }

    // string type ----------------------------------------------------------------------------
    struct str_t
    {
        const char* m_ascii; // pointer to an ascii string
        u32         m_str;   // ptr[m_str] is the first character
        u32         m_end;   // ptr[m_end] is one past the last character
        u32         m_eos;   // ptr[m_eos] is the end of the string but always points to a terminator
        u8          m_flags; // flags (owner/ref, temporary/permanent)
    };

// NULL string
#define str_null ((str_t){"", 0, 0, 0, 0, 0})

    // string properties ----------------------------------------------------------------------
    inline s32         str_len(str_t s) { return s.m_end - s.m_str; }
    inline const char* str_ptr(str_t s) { return s.m_ascii + s.m_str; }
    inline const char* str_end(str_t s) { return s.ptr + s.m_end; }
    inline bool        str_is_empty(str_t s) { return s.m_str == s.m_end; }
    inline bool        str_is_owner(str_t s) { return (s.m_flags & 1) == 1; }
    inline bool        str_is_ref(str_t s) { return !str_is_owner(s); }

    // string memory control -------------------------------------------------------------------
    str_t str_temporary(s32 max_len);
    str_t str_permanent(s32 max_len);
    void  str_free(str_t& s);

    // string movements -----------------------------------------------------------------------
    void  str_assign(str_t* dst, str_t src); // assign the string, copying if necessary
    str_t str_move(str_t* ps);               // move the string, resetting the source to str_null
    str_t str_pass(str_t* ps);               // pass ownership of the string
    void  str_swap(str_t* s1, str_t* s2);    // swap two string objects

    // string helpers --------------------------------------------------------------------------
    // reset the string to str_null
    inline void str_clear(str_t* ps) { str_assign(ps, str_null); }

    // string comparisons --------------------------------------------------------------------
    int         str_cmp(str_t s1, str_t s2, bool case_sensitive = true); // compare two strings lexicographically
    inline bool str_eq(str_t s1, str_t s2, bool case_sensitive = true) { return str_cmp(s1, s2, case_sensitive) == 0; }
    bool        str_has_prefix(str_t s, str_t prefix, bool case_sensitive = true); // test for prefix
    bool        str_has_suffix(str_t s, str_t suffix, bool case_sensitive = true); // test for suffix
    bool        str_contains(str_t s, str_t substr, bool case_sensitive = true);   // test for substring
    str_t       str_find(str_t s, str_t substr, bool case_sensitive = true);       // find first occurrence of substring
    str_t       str_find_last(str_t s, str_t substr, bool case_sensitive = true);  // find last occurrence of substring
    str_t       str_find_one_of(str_t s, str_t chars, bool case_sensitive = true); // find first occurrence of any of the given characters

    // string trimming ----------------------------------------------------------------------
    str_t str_trim_left(str_t s);                                   // trim whitespace from the left
    str_t str_trim_right(str_t s);                                  // trim whitespace from the right
    str_t str_trim(str_t s);                                        // trim whitespace from both ends
    str_t trimQuotes(str_t s);                                      // trim quotes from both ends if present
    str_t trimQuotes(str_t s, char quoteChar);                      // trim specified quote char from both ends if present
    str_t trimDelimiters(str_t s, char leftDelim, char rightDelim); // trim specified delimiters from both ends if present

    // from and to conversions ----------------------------------------------------------------
    str_t str_to_bool(str_t s, bool* outValue);
    str_t str_to_s32(str_t s, s32* outValue, s32 base = 10);
    str_t str_to_u32(str_t s, u32* outValue, s32 base = 10);
    str_t str_to_f32(str_t s, f32* outValue);

    void to_string(str_t* dest, bool value);
    void to_string(str_t* dest, s32 value, s32 base = 10);
    void to_string(str_t* dest, u32 value, s32 base = 10);
    void to_string(str_t* dest, f32 value, s32 num_fractional_digits = 2);

    // string composition ------------------------------------------------------------------
    // implementation helpers
    int str_copy(str_t* dest, str_t s);

    // implementation helpers
    int str_cat(str_t* dest, str_t src);
    int str_cat(str_t* dest, str_t* src, s32 count);

    // implementation helpers
    int str_join_range(str_t* dest, str_t sep, const str_t* src_array, s32 array_count);

    // constructors ----------------------------------------------------------------------------
    str_t str_ref(const char* s);                  // create a reference to the given null-terminated string
    str_t str_ref_chars(const char* s, s32 n);     // create a reference to the given range of chars
    str_t str_acquire(const char* s);              // take ownership of the given string
    str_t str_acquire_chars(const char* s, s32 n); // take ownership of the given range of chars

} // namespace ncore

#endif // __RDNO_CORE_STR_H__