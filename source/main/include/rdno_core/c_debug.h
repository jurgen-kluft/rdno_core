#ifndef __RDNO_CORE_DEBUG_H__
#define __RDNO_CORE_DEBUG_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    //==============================================================================
    // The debug API
    //==============================================================================
    class asserthandler_t
    {
    public:
        virtual ~asserthandler_t() {}
        virtual bool handle_assert(const char* fileName, s32 lineNumber, const char* exprString, const char* messageString) = 0;
    };

    //------------------------------------------------------------------------------
    void gSetAssertHandler(asserthandler_t* handler);

    //------------------------------------------------------------------------------
    // Summary:
    //     Main assert handler.
    // Arguments:
    //     flags                - Reference to a local copy of flags for the assert.
    //     fileName             - File name in which the assert happen
    //     lineNumber           - Line number where the assert happen
    //     exprString           - Expression of the assert
    //     messageString        - Additional string containing information about the
    //                            assert.
    // Returns:
    //     bool - TRUE when the program should be halted FALSE other wise
    // Description:
    //     This is the main entry of an assert, beyond this it will be dispatched to
    //     the x_debug object associated with the current thread.
    // See Also:
    //     ASSERTS DVERIFY
    //------------------------------------------------------------------------------
#ifdef D_ASSERT
    extern bool gAssertHandler(const char* fileName, s32 lineNumber, const char* exprString, const char* messageString);
#endif

//==============================================================================
// D_BREAK & D_NOP
//==============================================================================
// This two macros are use mainly for debugging. The nop is used more to
// separate a block of code from another. Very useful when looking at the
// assembly code.
//==============================================================================
#ifdef D_BREAK
#    undef D_BREAK
#endif

//------------------------------------------------------------------------------
// Summary:
//     Stop the program with a break point.
// Description:
//     The macro D_BREAK will cause a debugger breakpoint if possible on any given
//     platform.  If a breakpoint cannot be caused, then a divide by zero will be
//     forced.  Note that the D_BREAK macro is highly platform specific.  The
//     implementation of D_BREAK on some platforms prevents it from being used
//     syntactically as an expression.  It can only be used as a statement.
// See Also:
//     NOP ASSERT
//------------------------------------------------------------------------------
#if defined(TARGET_PC) && defined(COMPILER_DEFAULT)
#    define D_BREAK         \
        {                   \
            __debugbreak(); \
        }
#endif

#if defined(TARGET_MAC) && defined(COMPILER_DEFAULT)
#    define D_BREAK           \
        {                     \
            __builtin_trap(); \
        }
#endif

#if defined(TARGET_ESP32) && defined(COMPILER_DEFAULT)
#    define D_BREAK           \
        {                     \
        }
#endif

#if !defined(D_BREAK)
#    error Unknown Platform/Compiler configuration for D_BREAK
#endif

//------------------------------------------------------------------------------
#ifdef D_NOP
#    undef D_NOP
#endif

    //------------------------------------------------------------------------------
    // Summary:
    //     Sets a D_NOP operation in the code assembly.
    // Description:
    //     Sets a D_NOP operation in the code assembly. This is commonly used for
    //     debugging. By adding nops allows to see the assembly clearly in code.
    // See Also:
    //     BREAK
    //------------------------------------------------------------------------------

#if defined(TARGET_PC) && defined(COMPILER_DEFAULT)
#    define D_NOP               \
        {                       \
            __emit(0x00000000); \
        }
#endif

#if defined(TARGET_MAC) && defined(COMPILER_DEFAULT)
#    define D_NOP {__asm nop}
#endif

#if defined(TARGET_LINUX) && defined(COMPILER_DEFAULT)
#    define D_NOP {__asm nop}
#endif

#if defined(TARGET_ESP32) && defined(COMPILER_DEFAULT)
#    define D_NOP {__asm nop}
#endif

#if !defined(D_NOP)
#    error Unknown Platform/Compiler configuration for D_NOP
#endif

//==============================================================================
//  Runtime validation support.
//==============================================================================
//
//  Most of the run-time validations are one form or another of an ASSERT.  So,
//  for lack of a better name, the presence of the compile time macro D_ASSERT
//  activates the optional run-time validations.  (And, of course, the absence
//  of D_ASSERT deactivates them.)
//
//  The following macros and functions are all designed to perform validation of
//  expected conditions at run-time.  Each takes an expression as the first
//  parameter.  The expression (expr) is expected to be TRUE whenever evaluated.
//
//      ASSERT  (expr)
//      ASSERTS (expr, message)
//
//      DVERIFY  (expr)
//      DVERIFYS (expr, message)
//
//  The macros ASSERT and ASSERTS completely evaporate in when D_ASSERT is not
//  defined.  Macros DVERIFY and DVERIFYS, lacking D_ASSERT, still evaluate the
//  expression, but do not validate the result.  Consider:
//
//      ASSERT(CriticalInitialization());    // EVIL without D_ASSERT!
//      DVERIFY(CriticalInitialization());    // Safe without D_ASSERT.
//
//  The ASSERTS and DVERIFYS macros accept a message string to assist problem
//  diagnosis.  Upon a run-time failure, the message is displayed.  For example:
//
//      ASSERTS(Radius >= 0.0f, "Radius must be non-negative.");
//
//  Available options:
//
//    - As previously mentioned, the macro D_ASSERT enables the validation
//      macros.  D_ASSERT should be always be present in debug configurations.
//
//==============================================================================
#ifdef ASSERT
#    undef ASSERT
#endif

#ifdef ASSERTS
#    undef ASSERTS
#endif

#ifdef DVERIFY
#    undef DVERIFY
#endif

#ifdef DVERIFYS
#    undef DVERIFYS
#endif

#ifdef D_ASSERT

#    define DVERIFY(expr)                                                                    \
        do                                                                                   \
        {                                                                                    \
            if (!((expr) != 0) && ncore::gAssertHandler(__FILE__, __LINE__, #expr, nullptr)) \
                D_BREAK;                                                                     \
        } while (0)

#    define DVERIFYS(expr, str)                                                          \
        do                                                                               \
        {                                                                                \
            if (!((expr) != 0) && ncore::gAssertHandler(__FILE__, __LINE__, #expr, str)) \
                D_BREAK;                                                                 \
        } while (0)

#    define DBOUNDS(v, l, h)                                                                                                     \
        do                                                                                                                       \
        {                                                                                                                        \
            if (!((v) >= (l) && (v) <= (h)) && ncore::gAssertHandler(__FILE__, __LINE__, #v ">=" #l " && " #v "<=" #h, nullptr)) \
                D_BREAK;                                                                                                         \
        } while (0)

#    define DASSERTCT(expr) \
        do                  \
        {        static_assert(expr, "");           \
        } while (0);

#    define DASSERTSL(level, expr, str)                                                                                       \
        do                                                                                                                    \
        {                                                                                                                     \
            if (ncore::ChooseDebugLevel(level) && ((expr) == false) && ncore::gAssertHandler(__FILE__, __LINE__, #expr, str)) \
                D_BREAK;                                                                                                      \
        } while (0)

#    define DASSERTL(level, expr)                                                                                                 \
        do                                                                                                                        \
        {                                                                                                                         \
            if (ncore::ChooseDebugLevel(level) && ((expr) == false) && ncore::gAssertHandler(__FILE__, __LINE__, #expr, nullptr)) \
                D_BREAK;                                                                                                          \
        } while (0)

#    define DASSERT(expr)                                                                    \
        do                                                                                   \
        {                                                                                    \
            if (!((expr) != 0) && ncore::gAssertHandler(__FILE__, __LINE__, #expr, nullptr)) \
                D_BREAK;                                                                     \
        } while (0)

#    define DASSERTS(expr, str)                                                          \
        do                                                                               \
        {                                                                                \
            if (!((expr) != 0) && ncore::gAssertHandler(__FILE__, __LINE__, #expr, str)) \
                D_BREAK;                                                                 \
        } while (0)

#    define ASSERTCT(expr)                    DASSERTCT(expr)
#    define ASSERTSL(level, expr, str)        DASSERTSL(level, expr, str)
#    define ASSERTL(level, expr)              DASSERTL(level, expr)
#    define ASSERT(expr)                      DASSERT(expr)
#    define ASSERT_OPEN_RANGE(_i, _min, _max) DASSERT(_i >= _min && _i < _max)
#    define ASSERTS(expr, str)                DASSERTS(expr, str)

#else

#    define DASSERTCT(expr)
#    define DASSERT(expr)               (void(0))
#    define DASSERTS(expr, str)         (void(0))
#    define DASSERTSL(level, expr, str) (void(0))
#    define DASSERTL(level, expr)       (void(0))
#    define ASSERTCT(expr)
#    define ASSERT(expr)                      (void(0))
#    define ASSERT_OPEN_RANGE(_i, _min, _max) (void(0))
#    define ASSERTS(expr, str)                (void(0))
#    define ASSERTSL(level, expr, str)        (void(0))
#    define ASSERTL(level, expr)              (void(0))

#    define DVERIFY(expr)       (expr)
#    define DVERIFYS(expr, str) (expr)
#    define DBOUNDS(v, l, h)

#endif

};  // namespace ncore

#endif  /// __CCORE_DEBUG_H__
