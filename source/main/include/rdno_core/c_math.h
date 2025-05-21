#ifndef __ACORE_MATH_AND_BIT_UTILS_H__
#define __ACORE_MATH_AND_BIT_UTILS_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "rdno_core/c_debug.h"

namespace ncore
{
#undef min
#undef max

#ifdef log2
#    undef log2
#endif

    namespace math
    {
        template <class T>
        T g_min(T integerA, T integerB);  // Return the smallest value of 2
        template <class T>
        T g_min(T integerA, T integerB, T integerC);  // Return the smallest value of 3
        template <class T>
        T g_max(T integerA, T integerB);  // Return the biggest value
        template <class T>
        T g_max(T integerA, T integerB, T integerC);  // Return the biggest value of 3

        template <class T>
        void g_sort(T& integerA, T& integerB);  // Return integerA = smallest value, integerB = biggest value

        template <class T>
        T g_sign(T integerA)
        {
            return (integerA > 0) ? 1 : ((integerA < 0) ? -1 : 0);
        }

        template <class T>
        T g_clamp(T integerA, T low, T high);  // Return the clamp value

        /// clamp sub-range inside of encompassing range
        template <class T>
        void g_clampRange(T& subfrom, T& subto, T enfrom, T ento)
        {
            g_sort(subfrom, subto);
            g_sort(enfrom, ento);
            g_clamp(subfrom, enfrom, ento);
            g_clamp(subto, enfrom, ento);
        }

        inline u32  g_align(u32 integer, u32 alignment);      // Return the aligned value of integer
        inline u32  g_align(u32 integer, s32 alignment);      // Return the aligned value of integer
        inline u32  g_alignDown(u32 integer, u32 alignment);  // Return the aligned value of integer
        inline u32  g_alignDown(u32 integer, s32 alignment);  // Return the aligned value of integer
        inline s32  g_alignUp(s32 integer, s32 alignment);    // Return the aligned value of integer
        inline u32  g_alignUp(u32 integer, u32 alignment);    // Return the aligned value of integer
        inline u32  g_alignUp(u32 integer, s32 alignment);    // Return the aligned value of integer
        inline u64  g_alignUp(u64 integer, u32 alignment);    // Return the aligned value of integer
        inline u64  g_alignUp(u64 integer, s32 alignment);    // Return the aligned value of integer
        inline bool g_isAligned(u32 integer, u32 alignment);  // Return True if integer is aligned
        inline bool g_isAligned(u32 integer, s32 alignment);  // Return True if integer is aligned
        inline bool g_isAligned(u64 integer, u32 alignment);  // Return True if integer is aligned
        inline bool g_isAligned(u64 integer, s32 alignment);  // Return True if integer is aligned

        inline s32  g_abs(s32 integer);                      // Return the absolute value of integer
        inline s32  g_neg(s32 integer, bool inDoNotNegate);  // Return the conditionally negated value of integer
        inline s32  g_sqr(s32 integer);                      // Return the square of integer
        inline u32  g_sqr(u32 integer);                      // Return the unsigned square of integer
        inline s32  g_mod(s32 integer, s32 inModuloValue);   // Return the modulo of integer using inModuloValue
        inline s32  g_gcd(s32 inIntegerA, s32 integerB);     // Greatest common denominator (biggest modulo value of both integers)
        inline s32  g_average(s32 integerA, s32 integerB);   // Calculate average of A & B without overflow or s64 use
        inline bool g_ispo2(s32 integer);                    // Check if integer is a power-of-two
        inline bool g_ispo2(u32 integer);                    // Check if integer is a power-of-two
        inline bool g_ispo2(u64 integer);                    // Check if integer is a power-of-two
        inline u32  g_ceilpo2(u32 integer);                  // Return the smallest power-of-two larger than integer
        inline u32  g_floorpo2(u32 integer);                 // Return the biggest power-of-two smaller than integer
        inline u32  g_next_power_of_two(u32 val) { return g_ceilpo2(val); }
        inline u32  g_bitReverse(u32 integer);  // Reverse bits in 32 bit word
        inline s8   g_countBits(u8 integer);    // count one bits in 8 bit word
        inline s8   g_countBits(u16 integer);   // count one bits in 16 bit word
        inline s8   g_countBits(u32 integer);   // count one bits in 32 bit word
        inline s8   g_countBits(u64 integer);   // count one bits in 32 bit word

        inline s8 g_ilog2(u32 integer);  // Log2 of a 32-bit integer
        inline s8 g_ilog2(s32 integer) { return g_ilog2((u32)integer); }
        inline s8 g_ilog2(u64 integer);  // Log2 of a 64-bit integer
        inline s8 g_ilog2(s64 integer) { return g_ilog2((u64)integer); }

        inline u64 g_getMaskForValue(u64 value);             // Return the mask of the value
        inline s8  g_countTrailingZeros(u8 integer);         // find the number of trailing zeros in 8-bit v
        inline s8  g_countLeadingZeros(u8 integer);          // find the number of trailing zeros in 8-bit v
        inline s8  g_countTrailingZeros(u16 integer);        // find the number of trailing zeros in 16-bit v
        inline s8  g_countLeadingZeros(u16 integer);         // find the number of leading zeros in 16-bit v
        inline s8  g_countTrailingZeros(u32 integer);        // find the number of trailing zeros in 32-bit v
        inline s8  g_countLeadingZeros(u32 integer);         // find the number of leading zeros in 32-bit v
        inline s8  g_countTrailingZeros(u64 integer);        // find the number of trailing zeros in 64-bit v
        inline s8  g_countLeadingZeros(u64 integer);         // find the number of leading zeros in 64-bit v
        inline u32 g_leastSignificantOneBit(u32 inInteger);  // Return v but with only the Least Significant Bit "1"
        inline u32 g_mostSignificantOneBit(u32 integer);     // Return v but with only the Most Significant Bit "1"
        inline s8  g_leastSignificantBit(u32 integer);       // Return the bit index of the Least Significant Bit "1"
        inline s8  g_mostSignificantBit(u32 integer);        // Return the bit index of the Most Significant Bit "1"
        inline s8  g_findFirstBit(u8 integer);               // find the bit position/index of the first bit from low to high
        inline s8  g_findLastBit(u8 integer);                // find the bit position/index of the first bit from high to low
        inline s8  g_findFirstBit(u16 integer);              // find the bit position/index of the first bit from low to high
        inline s8  g_findLastBit(u16 integer);               // find the bit position/index of the first bit from high to low
        inline s8  g_findFirstBit(u32 integer);              // find the bit position/index of the first bit from low to high
        inline s8  g_findLastBit(u32 integer);               // find the bit position/index of the first bit from high to low
        inline s8  g_findFirstBit(u64 integer);              // find the bit position/index of the first bit from low to high
        inline s8  g_findLastBit(u64 integer);               // find the bit position/index of the first bit from high to low
        inline u32 g_rol32(u32 integer, u32 shift);          // Roll all the bits in integer to the left by shift number of bits
        inline u32 g_ror32(u32 integer, u32 shift);          // Roll all the bits in integer to the right by shift number of bits

    }  // namespace math
};  // namespace ncore

//==============================================================================
// INLINES
//==============================================================================
#include "rdno_core/private/c_math_inline.h"
#include "rdno_core/private/c_math_inline_generic.h"

#endif  // 
