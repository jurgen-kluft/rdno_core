#ifndef __RDNO_CORE_TARGET_COMPILER_V2_H__
#define __RDNO_CORE_TARGET_COMPILER_V2_H__
#pragma once

namespace ncore
{
    // ---------------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------------
    //  COMPILER
    // ---------------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------------

    //  Currently supported defines include:
    //      CC_COMPILER_GNUC
    //      CC_COMPILER_ARM
    //      CC_COMPILER_EDG
    //      CC_COMPILER_SN
    //      CC_COMPILER_MSVC
    //      CC_COMPILER_METROWERKS
    //      CC_COMPILER_INTEL
    //      CC_COMPILER_BORLANDC
    //      CC_COMPILER_IBM
    //      CC_COMPILER_QNX
    //      CC_COMPILER_GREEN_HILLS
    //      CC_COMPILER_CLANG
    //      CC_COMPILER_CLANG_CL
    //
    //      CC_COMPILER_VERSION = <integer>
    //      CC_COMPILER_NAME = <string>
    //      CC_COMPILER_STRING = <string>
    //
    //   C++98/03 functionality
    //      CC_COMPILER_NO_STATIC_CONSTANTS
    //      CC_COMPILER_NO_TEMPLATE_SPECIALIZATION
    //      CC_COMPILER_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    //      CC_COMPILER_NO_MEMBER_TEMPLATES
    //      CC_COMPILER_NO_MEMBER_TEMPLATE_SPECIALIZATION
    //      CC_COMPILER_NO_TEMPLATE_TEMPLATES
    //      CC_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS
    //      CC_COMPILER_NO_VOID_RETURNS
    //      CC_COMPILER_NO_COVARIANT_RETURN_TYPE
    //      CC_COMPILER_NO_DEDUCED_TYPENAME
    //      CC_COMPILER_NO_ARGUMENT_DEPENDENT_LOOKUP
    //      CC_COMPILER_NO_EXCEPTION_STD_NAMESPACE
    //      CC_COMPILER_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
    //      CC_COMPILER_NO_RTTI
    //      CC_COMPILER_NO_EXCEPTIONS
    //      CC_COMPILER_NO_UNWIND
    //      CC_COMPILER_NO_STANDARD_CPP_LIBRARY
    //      CC_COMPILER_NO_STATIC_VARIABLE_INIT
    //      CC_COMPILER_NO_STATIC_FUNCTION_INIT
    //      CC_COMPILER_NO_VARIADIC_MACROS
    //
    //   C++11 functionality
    //      CC_COMPILER_NO_RVALUE_REFERENCES
    //      CC_COMPILER_NO_EXTERN_TEMPLATE
    //      CC_COMPILER_NO_CONSTEXPR
    //      CC_COMPILER_NO_OVERRIDE
    //      CC_COMPILER_NO_INHERITANCE_FINAL
    //      CC_COMPILER_NO_NULLPTR
    //      CC_COMPILER_NO_AUTO
    //      CC_COMPILER_NO_DECLTYPE
    //      CC_COMPILER_NO_DEFAULTED_FUNCTIONS
    //      CC_COMPILER_NO_DELETED_FUNCTIONS
    //      CC_COMPILER_NO_LAMBDA_EXPRESSIONS
    //      CC_COMPILER_NO_TRAILING_RETURN_TYPES
    //      CC_COMPILER_NO_STRONGLY_TYPED_ENUMS
    //      CC_COMPILER_NO_FORWARD_DECLARED_ENUMS
    //      CC_COMPILER_NO_VARIADIC_TEMPLATES
    //      CC_COMPILER_NO_TEMPLATE_ALIASES
    //      CC_COMPILER_NO_INITIALIZER_LISTS
    //      CC_COMPILER_NO_NORETURN
    //      CC_COMPILER_NO_CARRIES_DEPENDENCY
    //      CC_COMPILER_NO_FALLTHROUGH
    //      CC_COMPILER_NO_NODISCARD
    //      CC_COMPILER_NO_MAYBE_UNUSED
    //      CC_COMPILER_NO_NONSTATIC_MEMBER_INITIALIZERS
    //      CC_COMPILER_NO_RIGHT_ANGLE_BRACKETS
    //      CC_COMPILER_NO_ALIGNOF
    //      CC_COMPILER_NO_ALIGNAS
    //      CC_COMPILER_NO_DELEGATING_CONSTRUCTORS
    //      CC_COMPILER_NO_INHERITING_CONSTRUCTORS
    //      CC_COMPILER_NO_USER_DEFINED_LITERALS
    //      CC_COMPILER_NO_STANDARD_LAYOUT_TYPES
    //      CC_COMPILER_NO_EXTENDED_SIZEOF
    //      CC_COMPILER_NO_INLINE_NAMESPACES
    //      CC_COMPILER_NO_UNRESTRICTED_UNIONS
    //      CC_COMPILER_NO_EXPLICIT_CONVERSION_OPERATORS
    //      CC_COMPILER_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS
    //      CC_COMPILER_NO_LOCAL_CLASS_TEMPLATE_PARAMETERS
    //      CC_COMPILER_NO_NOEXCEPT
    //      CC_COMPILER_NO_RAW_LITERALS
    //      CC_COMPILER_NO_UNICODE_STRING_LITERALS
    //      CC_COMPILER_NO_NEW_CHARACTER_TYPES
    //      CC_COMPILER_NO_UNICODE_CHAR_NAME_LITERALS
    //      CC_COMPILER_NO_UNIFIED_INITIALIZATION_SYNTAX
    //      CC_COMPILER_NO_EXTENDED_FRIEND_DECLARATIONS
    //
    //   C++14 functionality
    //      CC_COMPILER_NO_VARIABLE_TEMPLATES
    //
    //   C++17 functionality
    //      CC_COMPILER_NO_INLINE_VARIABLES
    //      CC_COMPILER_NO_ALIGNED_NEW
    //
    //   C++20 functionality
    //      CC_COMPILER_NO_DESIGNATED_INITIALIZERS
    //
    // -----------------------------------------------------------------------------
    //
    //  Supplemental documentation
    //      CC_COMPILER_NO_STATIC_CONSTANTS
    //          Code such as this is legal, but some compilers fail to compile it:
    //              struct A{ static const a = 1; };
    //
    //      CC_COMPILER_NO_TEMPLATE_SPECIALIZATION
    //          Some compilers fail to allow template specialization, such as with this:
    //              template<class U> void DoSomething(U u);
    //              void DoSomething(int x);
    //
    //      CC_COMPILER_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    //          Some compilers fail to allow partial template specialization, such as with this:
    //              template <class T, class Allocator> class vector{ };         // Primary templated class.
    //              template <class Allocator> class vector<bool, Allocator>{ }; // Partially specialized version.
    //
    //      CC_COMPILER_NO_MEMBER_TEMPLATES
    //          Some compilers fail to allow member template functions such as this:
    //              struct A{ template<class U> void DoSomething(U u); };
    //
    //      CC_COMPILER_NO_MEMBER_TEMPLATE_SPECIALIZATION
    //          Some compilers fail to allow member template specialization, such as with this:
    //              struct A{
    //                  template<class U> void DoSomething(U u);
    //                  void DoSomething(int x);
    //              };
    //
    //      CC_COMPILER_NO_TEMPLATE_TEMPLATES
    //          Code such as this is legal:
    //              template<typename T, template<typename> class U>
    //              U<T> SomeFunction(const U<T> x) { return x.DoSomething(); }
    //
    //      CC_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS
    //          Some compilers fail to compile templated friends, as with this:
    //              struct A{ template<class U> friend class SomeFriend; };
    //          This is described in the C++ Standard at 14.5.3.
    //
    //      CC_COMPILER_NO_VOID_RETURNS
    //           This is legal C++:
    //               void DoNothing1(){ };
    //               void DoNothing2(){ return DoNothing1(); }
    //
    //      CC_COMPILER_NO_COVARIANT_RETURN_TYPE
    //          See the C++ standard sec 10.3,p5.
    //
    //      CC_COMPILER_NO_DEDUCED_TYPENAME
    //          Some compilers don't support the use of 'typename' for
    //          dependent types in deduced contexts, as with this:
    //              template <class T> void Function(T, typename T::type);
    //
    //      CC_COMPILER_NO_ARGUMENT_DEPENDENT_LOOKUP
    //          Also known as Koenig lookup. Basically, if you have a function
    //          that is a namespace and you call that function without prefixing
    //          it with the namespace the compiler should look at any arguments
    //          you pass to that function call and search their namespace *first*
    //          to see if the given function exists there.
    //
    //      CC_COMPILER_NO_EXCEPTION_STD_NAMESPACE
    //          <exception> is in namespace std. Some std libraries fail to
    //          put the contents of <exception> in namespace std. The following
    //          code should normally be legal:
    //              void Function(){ std::terminate(); }
    //
    //      CC_COMPILER_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
    //          Some compilers fail to execute DoSomething() properly, though they
    //          succeed in compiling it, as with this:
    //              template <int i>
    //              bool DoSomething(int j){ return i == j; };
    //              DoSomething<1>(2);
    //
    //      CC_COMPILER_NO_EXCEPTIONS
    //          The compiler is configured to disallow the use of try/throw/catch
    //          syntax (often to improve performance). Use of such syntax in this
    //          case will cause a compilation error.
    //
    //      CC_COMPILER_NO_UNWIND
    //          The compiler is configured to allow the use of try/throw/catch
    //          syntax and behaviour but disables the generation of stack unwinding
    //          code for responding to exceptions (often to improve performance).
    //
    // ---------------------------------------------------------------------------*/

// Note: This is used to generate the CC_COMPILER_STRING macros
#ifndef CC_INTERNAL_STRINGIZE
    #define CC_INTERNAL_STRINGIZE(x) CC_INTERNAL_PRIMITIVE_STRINGIZE(x)
#endif
#ifndef CC_INTERNAL_PRIMITIVE_STRINGIZE
    #define CC_INTERNAL_PRIMITIVE_STRINGIZE(x) #x
#endif

// CC_COMPILER_HAS_FEATURE
#ifndef CC_COMPILER_HAS_FEATURE
    #if defined(__clang__)
        #define CC_COMPILER_HAS_FEATURE(x) __has_feature(x)
    #else
        #define CC_COMPILER_HAS_FEATURE(x) 0
    #endif
#endif

// CC_COMPILER_HAS_BUILTIN
#ifndef CC_COMPILER_HAS_BUILTIN
    #if defined(__clang__) || (defined(__GNUC__) && (__GNUC__ >= 10))
        #define CC_COMPILER_HAS_BUILTIN(x) __has_builtin(x)
    #else
        #define CC_COMPILER_HAS_BUILTIN(x) 0
    #endif
#endif

// EDG (EDG compiler front-end, used by other compilers such as SN)
#if defined(__EDG_VERSION__)
    #define CC_COMPILER_EDG 1

    #if defined(_MSC_VER)
        #define CC_COMPILER_EDG_VC_MODE 1
    #endif
    #if defined(__GNUC__)
        #define CC_COMPILER_EDG_GCC_MODE 1
    #endif
#endif

// CC_COMPILER_CPP11_ENABLED
//
// Defined as 1 if the compiler has its available C++11 support enabled, else undefined.
// This does not mean that all of C++11 or any particular feature of C++11 is supported
// by the compiler. It means that whatever C++11 support the compiler has is enabled.
// This also includes existing and older compilers that still identify C++11 as C++0x.
//
// We cannot use (__cplusplus >= 201103L) alone because some compiler vendors have
// decided to not define __cplusplus like thus until they have fully completed their
// C++11 support.
//
#if !defined(CC_COMPILER_CPP11_ENABLED) && defined(__cplusplus)
    #if (__cplusplus >= 201103L)  // Clang and GCC defines this like so in C++11 mode.
        #define CC_COMPILER_CPP11_ENABLED 1
    #elif defined(__GNUC__) && defined(__GXX_EXPERIMENTAL_CXX0X__)
        #define CC_COMPILER_CPP11_ENABLED 1
    #elif defined(_MSC_VER) && _MSC_VER >= 1600  // Microsoft unilaterally enables its C++11 support; there is no way to disable it.
        #define CC_COMPILER_CPP11_ENABLED 1
    #elif defined(__EDG_VERSION__)  // && ???
            // To do: Is there a generic way to determine this?
    #endif
#endif

// CC_COMPILER_CPP14_ENABLED
//
// Defined as 1 if the compiler has its available C++14 support enabled, else undefined.
// This does not mean that all of C++14 or any particular feature of C++14 is supported
// by the compiler. It means that whatever C++14 support the compiler has is enabled.
//
// We cannot use (__cplusplus >= 201402L) alone because some compiler vendors have
// decided to not define __cplusplus like thus until they have fully completed their
// C++14 support.
#if !defined(CC_COMPILER_CPP14_ENABLED) && defined(__cplusplus)
    #if (__cplusplus >= 201402L)  // Clang and GCC defines this like so in C++14 mode.
        #define CC_COMPILER_CPP14_ENABLED 1
    #elif defined(_MSC_VER) && (_MSC_VER >= 1900)  // VS2015+
        #define CC_COMPILER_CPP14_ENABLED 1
    #endif
#endif

// CC_COMPILER_CPP17_ENABLED
//
// Defined as 1 if the compiler has its available C++17 support enabled, else undefined.
// This does not mean that all of C++17 or any particular feature of C++17 is supported
// by the compiler. It means that whatever C++17 support the compiler has is enabled.
//
// We cannot use (__cplusplus >= 201703L) alone because some compiler vendors have
// decided to not define __cplusplus like thus until they have fully completed their
// C++17 support.
#if !defined(CC_COMPILER_CPP17_ENABLED) && defined(__cplusplus)
    #if (__cplusplus >= 201703L)
        #define CC_COMPILER_CPP17_ENABLED 1
    #elif defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)  // C++17+
        #define CC_COMPILER_CPP17_ENABLED 1
    #endif
#endif

// CC_COMPILER_CPP20_ENABLED
//
// Defined as 1 if the compiler has its available C++20 support enabled, else undefined.
// This does not mean that all of C++20 or any particular feature of C++20 is supported
// by the compiler. It means that whatever C++20 support the compiler has is enabled.
//
// We cannot use (__cplusplus >= 202002L) alone because some compiler vendors have
// decided to not define __cplusplus like thus until they have fully completed their
// C++20 support.
#if !defined(CC_COMPILER_CPP20_ENABLED) && defined(__cplusplus)
    #if (__cplusplus >= 202002L)
        #define CC_COMPILER_CPP20_ENABLED 1
    #elif defined(_MSVC_LANG) && (_MSVC_LANG >= 202003L)  // C++20+
        #define CC_COMPILER_CPP20_ENABLED 1
    #endif
#endif

#if defined(__ARMCC_VERSION)
    // Note that this refers to the ARM RVCT compiler (armcc or armcpp), but there
    // are other compilers that target ARM processors, such as GCC and Microsoft VC++.
    // If you want to detect compiling for the ARM processor, check for CC_PROCESSOR_ARM
    // being defined.
    // This compiler is also identified by defined(__CC_ARM) || defined(__ARMCC__).
    #define CC_COMPILER_RVCT    1
    #define CC_COMPILER_ARM     1
    #define CC_COMPILER_VERSION __ARMCC_VERSION
    #define CC_COMPILER_NAME    "RVCT"
        // #define CC_COMPILER_STRING (defined below)

// Clang's GCC-compatible driver.
#elif defined(__clang__) && !defined(_MSC_VER)
    #define CC_COMPILER_CLANG   1
    #define CC_COMPILER_VERSION (__clang_major__ * 100 + __clang_minor__)
    #define CC_COMPILER_NAME    "clang"
    #define CC_COMPILER_STRING  CC_COMPILER_NAME __clang_version__
    #define COMPILER_DEFAULT 1

// GCC (a.k.a. GNUC)
#elif defined(__GNUC__)  // GCC compilers exist for many platforms.
    #define CC_COMPILER_GNUC    1
    #define CC_COMPILER_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
    #define CC_COMPILER_NAME    "GCC"
    #define CC_COMPILER_STRING  CC_COMPILER_NAME " compiler, version " CC_INTERNAL_STRINGIZE(__GNUC__) "." CC_INTERNAL_STRINGIZE(__GNUC_MINOR__)
    #define COMPILER_DEFAULT 1

    #if (__GNUC__ == 2) && (__GNUC_MINOR__ < 95)  // If GCC < 2.95...
        #define CC_COMPILER_NO_MEMBER_TEMPLATES 1
    #endif
    #if (__GNUC__ == 2) && (__GNUC_MINOR__ <= 97)  // If GCC <= 2.97...
        #define CC_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS 1
    #endif
    #if (__GNUC__ == 3) && ((__GNUC_MINOR__ == 1) || (__GNUC_MINOR__ == 2))  // If GCC 3.1 or 3.2 (but not pre 3.1 or post 3.2)...
        #define CC_COMPILER_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS 1
    #endif

// Borland C++
#elif defined(__BORLANDC__)
    #define CC_COMPILER_BORLANDC 1
    #define CC_COMPILER_VERSION  __BORLANDC__
    #define CC_COMPILER_NAME     "Borland C"
        // #define CC_COMPILER_STRING (defined below)

    #if (__BORLANDC__ <= 0x0550)  // If Borland C++ Builder 4 and 5...
        #define CC_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS 1
    #endif
    #if (__BORLANDC__ >= 0x561) && (__BORLANDC__ < 0x600)
        #define CC_COMPILER_NO_MEMBER_FUNCTION_SPECIALIZATION 1
    #endif

// Intel C++
// The Intel Windows compiler masquerades as VC++ and defines _MSC_VER.
// The Intel compiler is based on the EDG compiler front-end.
#elif defined(__ICL) || defined(__ICC)
    #define CC_COMPILER_INTEL 1

    // Should we enable the following? We probably should do so since enabling it does a lot more good than harm
    // for users. The Intel Windows compiler does a pretty good job of emulating VC++ and so the user would likely
    // have to handle few special cases where the Intel compiler doesn't emulate VC++ correctly.
    #if defined(_MSC_VER)
        #define CC_COMPILER_MSVC      1
        #define CC_COMPILER_MICROSOFT 1
    #endif

    // Should we enable the following? This isn't as clear because as of this writing we don't know if the Intel
    // compiler truly emulates GCC well enough that enabling this does more good than harm.
    #if defined(__GNUC__)
        #define CC_COMPILER_GNUC 1
    #endif

    #if defined(__ICL)
        #define CC_COMPILER_VERSION __ICL
    #elif defined(__ICC)
        #define CC_COMPILER_VERSION __ICC
    #endif
    #define CC_COMPILER_NAME "Intel C++"
    #if defined(_MSC_VER)
        #define CC_COMPILER_STRING CC_COMPILER_NAME " compiler, version " CC_INTERNAL_STRINGIZE(CC_COMPILER_VERSION) ", EDG version " CC_INTERNAL_STRINGIZE(__EDG_VERSION__) ", VC++ version " CC_INTERNAL_STRINGIZE(_MSC_VER)
    #elif defined(__GNUC__)
        #define CC_COMPILER_STRING CC_COMPILER_NAME " compiler, version " CC_INTERNAL_STRINGIZE(CC_COMPILER_VERSION) ", EDG version " CC_INTERNAL_STRINGIZE(__EDG_VERSION__) ", GCC version " CC_INTERNAL_STRINGIZE(__GNUC__)
    #else
        #define CC_COMPILER_STRING CC_COMPILER_NAME " compiler, version " CC_INTERNAL_STRINGIZE(CC_COMPILER_VERSION) ", EDG version " CC_INTERNAL_STRINGIZE(__EDG_VERSION__)
    #endif

#elif defined(_MSC_VER)
    #define CC_COMPILER_MICROSOFT 1
    #define CC_COMPILER_VERSION   _MSC_VER
    #define CC_COMPILER_NAME      "Microsoft Visual C++"
        // #define CC_COMPILER_STRING (defined below)
    #define COMPILER_DEFAULT 1

    #if defined(__clang__)
            // Clang's MSVC-compatible driver.
        #define CC_COMPILER_CLANG_CL 1
    #endif

    #define CC_STANDARD_LIBRARY_MSVC      1
    #define CC_STANDARD_LIBRARY_MICROSOFT 1

    #if (_MSC_VER <= 1200)  // If VC6.x and earlier...
        #if (_MSC_VER < 1200)
            #define CC_COMPILER_MSVCOLD 1
        #else
            #define CC_COMPILER_MSVC6 1
        #endif

        #if (_MSC_VER < 1200)  // If VC5.x or earlier...
            #define CC_COMPILER_NO_TEMPLATE_SPECIALIZATION 1
        #endif
        #define CC_COMPILER_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS 1  // The compiler compiles this OK, but executes it wrong. Fixed in VC7.0
        #define CC_COMPILER_NO_VOID_RETURNS                         1  // The compiler fails to compile such cases. Fixed in VC7.0
        #define CC_COMPILER_NO_EXCEPTION_STD_NAMESPACE              1  // The compiler fails to compile such cases. Fixed in VC7.0
        #define CC_COMPILER_NO_DEDUCED_TYPENAME                     1  // The compiler fails to compile such cases. Fixed in VC7.0
        #define CC_COMPILER_NO_STATIC_CONSTANTS                     1  // The compiler fails to compile such cases. Fixed in VC7.0
        #define CC_COMPILER_NO_COVARIANT_RETURN_TYPE                1  // The compiler fails to compile such cases. Fixed in VC7.1
        #define CC_COMPILER_NO_ARGUMENT_DEPENDENT_LOOKUP            1  // The compiler compiles this OK, but executes it wrong. Fixed in VC7.1
        #define CC_COMPILER_NO_TEMPLATE_TEMPLATES                   1  // The compiler fails to compile such cases. Fixed in VC7.1
        #define CC_COMPILER_NO_TEMPLATE_PARTIAL_SPECIALIZATION      1  // The compiler fails to compile such cases. Fixed in VC7.1
        #define CC_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS              1  // The compiler fails to compile such cases. Fixed in VC7.1
            // #define CC_COMPILER_NO_MEMBER_TEMPLATES 1                       // VC6.x supports member templates properly 95% of the time. So do we flag the remaining 5%?
            // #define CC_COMPILER_NO_MEMBER_TEMPLATE_SPECIALIZATION 1         // VC6.x supports member templates properly 95% of the time. So do we flag the remaining 5%?

    #elif (_MSC_VER <= 1300)  // If VC7.0 and earlier...
        #define CC_COMPILER_MSVC7 1

        #define CC_COMPILER_NO_COVARIANT_RETURN_TYPE           1  // The compiler fails to compile such cases. Fixed in VC7.1
        #define CC_COMPILER_NO_ARGUMENT_DEPENDENT_LOOKUP       1  // The compiler compiles this OK, but executes it wrong. Fixed in VC7.1
        #define CC_COMPILER_NO_TEMPLATE_TEMPLATES              1  // The compiler fails to compile such cases. Fixed in VC7.1
        #define CC_COMPILER_NO_TEMPLATE_PARTIAL_SPECIALIZATION 1  // The compiler fails to compile such cases. Fixed in VC7.1
        #define CC_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS         1  // The compiler fails to compile such cases. Fixed in VC7.1
        #define CC_COMPILER_NO_MEMBER_FUNCTION_SPECIALIZATION  1  // This is the case only for VC7.0 and not VC6 or VC7.1+. Fixed in VC7.1
            // #define CC_COMPILER_NO_MEMBER_TEMPLATES 1                       // VC7.0 supports member templates properly 95% of the time. So do we flag the remaining 5%?

    #elif (_MSC_VER < 1400)  // VS2003       _MSC_VER of 1300 means VC7 (VS2003)
            // The VC7.1 and later compiler is fairly close to the C++ standard
        // and thus has no compiler limitations that we are concerned about.
        #define CC_COMPILER_MSVC
        #define CC_COMPILER_MSVC_VERSION 2003
        #define CC_COMPILER_MSVC7_2003   1
        #define CC_COMPILER_MSVC7_1      1

    #elif (_MSC_VER < 1500)  // VS2005       _MSC_VER of 1400 means VC8 (VS2005)
        #define CC_COMPILER_MSVC
        #define CC_COMPILER_MSVC_VERSION 2005
        #define CC_COMPILER_MSVC8_2005   1
        #define CC_COMPILER_MSVC8_0      1

    #elif (_MSC_VER < 1600)  // VS2008.      _MSC_VER of 1500 means VC9 (VS2008)
        #define CC_COMPILER_MSVC
        #define CC_COMPILER_MSVC_VERSION 2008
        #define CC_COMPILER_MSVC9_2008   1
        #define CC_COMPILER_MSVC9_0      1

    #elif (_MSC_VER < 1700)  // VS2010       _MSC_VER of 1600 means VC10 (VS2010)
        #define CC_COMPILER_MSVC
        #define CC_COMPILER_MSVC_VERSION 2010
        #define CC_COMPILER_MSVC_2010    1
        #define CC_COMPILER_MSVC10_0     1

    #elif (_MSC_VER < 1800)  // VS2012       _MSC_VER of 1700 means VS2011/VS2012
        #define CC_COMPILER_MSVC
        #define CC_COMPILER_MSVC_VERSION 2012
        #define CC_COMPILER_MSVC_2011    1  // Microsoft changed the name to VS2012 before shipping, despite referring to it as VS2011 up to just a few weeks before shipping.
        #define CC_COMPILER_MSVC11_0     1
        #define CC_COMPILER_MSVC_2012    1
        #define CC_COMPILER_MSVC12_0     1

    #elif (_MSC_VER < 1900)  // VS2013       _MSC_VER of 1800 means VS2013
        #define CC_COMPILER_MSVC
        #define CC_COMPILER_MSVC_VERSION 2013
        #define CC_COMPILER_MSVC_2013    1
        #define CC_COMPILER_MSVC13_0     1

    #elif (_MSC_VER < 1911)  // VS2015       _MSC_VER of 1900 means VS2015
        #define CC_COMPILER_MSVC
        #define CC_COMPILER_MSVC_VERSION 2015
        #define CC_COMPILER_MSVC_2015    1
        #define CC_COMPILER_MSVC14_0     1

    #elif (_MSC_VER < 1920)  // VS2017       _MSC_VER of 1910 means VS2017
        #define CC_COMPILER_MSVC
        #define CC_COMPILER_MSVC_VERSION 2017
        #define CC_COMPILER_MSVC_2017    1
        #define CC_COMPILER_MSVC15_0     1
    #elif (_MSC_VER < 1930)  // VS2019       _MSC_VER of 1920 means VS2019
        #define CC_COMPILER_MSVC
        #define CC_COMPILER_MSVC_VERSION 2019
        #define CC_COMPILER_MSVC_2019    1
        #define CC_COMPILER_MSVC16_0     1
    #elif (_MSC_VER < 1942)  // VS2022       _MSC_VER of 1930 means VS2022
        #define CC_COMPILER_MSVC
        #define CC_COMPILER_MSVC_VERSION 2022
        #define CC_COMPILER_MSVC_2022    1
        #define CC_COMPILER_MSVC17_0     1
    #else
        #define CC_COMPILER_MSVC
        #define CC_COMPILER_MSVC_VERSION 2022
        #define CC_COMPILER_MSVC_2022    1
        #define CC_COMPILER_MSVC17_0     1
    #endif
// IBM
#elif defined(__xlC__)
    #define CC_COMPILER_IBM     1
    #define CC_COMPILER_NAME    "IBM XL C"
    #define CC_COMPILER_VERSION __xlC__
    #define CC_COMPILER_STRING  "IBM XL C compiler, version " CC_INTERNAL_STRINGIZE(__xlC__)

// Unknown
#else  // Else the compiler is unknown

    #define CC_COMPILER_VERSION 0
    #define CC_COMPILER_NAME    "Unknown"

#endif

#ifndef CC_COMPILER_STRING
    #define CC_COMPILER_STRING CC_COMPILER_NAME " compiler, version " CC_INTERNAL_STRINGIZE(CC_COMPILER_VERSION)
#endif

// CC_COMPILER_NO_RTTI
//
// If CC_COMPILER_NO_RTTI is defined, then RTTI (run-time type information)
// is not available (possibly due to being disabled by the user).
//
#if defined(__EDG_VERSION__) && !defined(__RTTI)
    #define CC_COMPILER_NO_RTTI 1
#elif defined(__clang__) && !CC_COMPILER_HAS_FEATURE(cxx_rtti)
    #define CC_COMPILER_NO_RTTI 1
#elif defined(__IBMCPP__) && !defined(__RTTI_ALL__)
    #define CC_COMPILER_NO_RTTI 1
#elif defined(__GXX_ABI_VERSION) && !defined(__GXX_RTTI)
    #define CC_COMPILER_NO_RTTI 1
#elif defined(_MSC_VER) && !defined(_CPPRTTI)
    #define CC_COMPILER_NO_RTTI 1
#elif defined(__ARMCC_VERSION) && defined(__TARGET_CPU_MPCORE) && !defined(__RTTI)
    #define CC_COMPILER_NO_RTTI 1
#endif

// CC_COMPILER_NO_EXCEPTIONS / CC_COMPILER_NO_UNWIND
//
// If CC_COMPILER_NO_EXCEPTIONS is defined, then the compiler is
// configured to not recognize C++ exception-handling statements
// such as try/catch/throw. Thus, when CC_COMPILER_NO_EXCEPTIONS is
// defined, code that attempts to use exception handling statements
// will usually cause a compilation error. If is often desirable
// for projects to disable exception handling because exception
// handling causes extra code and/or data generation which might
// not be needed, especially if it is known that exceptions won't
// be happening. When writing code that is to be portable between
// systems of which some enable exception handling while others
// don't, check for CC_COMPILER_NO_EXCEPTIONS being defined.
//
#if !defined(CC_COMPILER_NO_EXCEPTIONS) && !defined(CC_COMPILER_NO_UNWIND)
    #if defined(CC_COMPILER_GNUC) && defined(_NO_EX)  // GCC on some platforms defines _NO_EX when exceptions are disabled.
        #define CC_COMPILER_NO_EXCEPTIONS 1

    #elif (defined(CC_COMPILER_CLANG) || defined(CC_COMPILER_GNUC) || defined(CC_COMPILER_INTEL) || defined(CC_COMPILER_RVCT)) && !defined(__EXCEPTIONS)  // GCC and most EDG-based compilers define __EXCEPTIONS when exception handling is enabled.
        #define CC_COMPILER_NO_EXCEPTIONS 1

    #elif (defined(CC_COMPILER_MSVC)) && !defined(_CPPUNWIND)
        #define CC_COMPILER_NO_UNWIND 1

    #endif  // CC_COMPILER_NO_EXCEPTIONS / CC_COMPILER_NO_UNWIND
#endif      // !defined(CC_COMPILER_NO_EXCEPTIONS) && !defined(CC_COMPILER_NO_UNWIND)

// ------------------------------------------------------------------------
// CC_DISABLE_ALL_VC_WARNINGS / CC_RESTORE_ALL_VC_WARNINGS
//
// Disable and re-enable all warning(s) within code.
//
// Example usage:
//     CC_DISABLE_ALL_VC_WARNINGS()
//     <code>
//     CC_RESTORE_ALL_VC_WARNINGS()
//
// This is duplicated from CCore's c_compiler_traits.h
#ifndef CC_DISABLE_ALL_VC_WARNINGS
    #if defined(_MSC_VER)
        #define CC_DISABLE_ALL_VC_WARNINGS() __pragma(warning(push, 0)) __pragma(warning(disable : 4244 4265 4267 4350 4472 4509 4548 4623 4710 4985 6320 4755 4625 4626 4702 4668))  // Some warnings need to be explicitly called out.
    #else
        #define CC_DISABLE_ALL_VC_WARNINGS()
    #endif
#endif

// This is duplicated from CCore's c_compiler_traits.h
#ifndef CC_RESTORE_ALL_VC_WARNINGS
    #if defined(_MSC_VER)
        #define CC_RESTORE_ALL_VC_WARNINGS() __pragma(warning(pop))
    #else
        #define CC_RESTORE_ALL_VC_WARNINGS()
    #endif
#endif

// CC_COMPILER_NO_ALIGNED_NEW
//
//
#if !defined(CC_COMPILER_NO_ALIGNED_NEW)
    #if defined(_HAS_ALIGNED_NEW) && _HAS_ALIGNED_NEW  // VS2017 15.5 Preview
                                                       // supported.
    #elif defined(CC_COMPILER_CPP17_ENABLED)
            // supported.
    #else
        #define CC_COMPILER_NO_ALIGNED_NEW 1
    #endif
#endif

// CC_COMPILER_NO_STANDARD_CPP_LIBRARY
//
// If defined, then the compiler doesn't provide a Standard C++ library.
//
#if defined(CC_PLATFORM_ANDROID)
        // Disabled because android_sdk packages currently
        // don't support linking STL libraries. Perhaps we can figure out what linker arguments
        // are needed for an app so we can manually specify them and then re-enable this code.
        // #include <android/api-level.h>
        //
        // #if (__ANDROID_API__ < 9) // Earlier versions of Android provide no std C++ STL implementation.
    #define CC_COMPILER_NO_STANDARD_CPP_LIBRARY 1
        // #endif
#endif

// CC_COMPILER_NO_VARIADIC_MACROS
//
// If defined, the compiler doesn't support C99/C++11 variadic macros.
// With a variadic macro, you can do this:
//     #define MY_PRINTF(format, ...) printf(format, __VA_ARGS__)
//
#if !defined(CC_COMPILER_NO_VARIADIC_MACROS)
    #if defined(_MSC_VER) && (_MSC_VER < 1500)  // If earlier than VS2008..
        #define CC_COMPILER_NO_VARIADIC_MACROS 1
    #elif defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__)) < 401  // If earlier than GCC 4.1..
        #define CC_COMPILER_NO_VARIADIC_MACROS 1
    #elif defined(CC_COMPILER_EDG)  // Includes other compilers
            // variadic macros are supported
    #endif
#endif

// CC_COMPILER_NO_RVALUE_REFERENCES
//
// If defined, the compiler doesn't fully support C++11 rvalue reference semantics.
// This applies to the compiler only and not the Standard Library in use with the compiler,
// which is required by the Standard to have some support itself.
//
#if !defined(CC_COMPILER_NO_RVALUE_REFERENCES)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (_MSC_VER >= 1600)                 // VS2010+
                                                                                                      // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 403)  // EDG 4.3+.
            // supported. Earlier EDG supported a subset of rvalue references. Implicit move constructors and assignment operators aren't supported until EDG 4.5.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && CC_COMPILER_HAS_FEATURE(cxx_rvalue_references)
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4005)  // GCC 4.5+
            // supported.
    #else
        #define CC_COMPILER_NO_RVALUE_REFERENCES 1
    #endif
#endif

// CC_COMPILER_NO_EXTERN_TEMPLATE
//
// If defined, the compiler doesn't support C++11 extern template.
// With extern templates, you can do this:
//     extern template void DoSomething(KnownType u);
//
#if !defined(CC_COMPILER_NO_EXTERN_TEMPLATE)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (_MSC_VER >= 1700)                 // VS2012+...
                                                                                                      // Extern template is supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 401)  // EDG 4.1+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && defined(__apple_build_version__) && (CC_COMPILER_VERSION >= 401)
            // Extern template is supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && !defined(__apple_build_version__)  // Clang other than Apple's Clang
            // Extern template is supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4006)       // GCC 4.6+
            // Extern template is supported.
    #else
        #define CC_COMPILER_NO_EXTERN_TEMPLATE 1
    #endif
#endif

// CC_COMPILER_NO_CONSTEXPR
//
// Refers to C++11 = constexpr (const expression) declarations.
//
#if !defined(CC_COMPILER_NO_CONSTEXPR)
    #if defined(CC_COMPILER_CPP11_ENABLED) && (defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1900))    // VS2015+... Not present in VC++ up to and including VS2013.
                                                                                                      // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 406)  // EDG 4.6+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && CC_COMPILER_HAS_FEATURE(cxx_constexpr)
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4006)  // GCC 4.6+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1900)  // VS 2015+
            // supported.
    #else
        #define CC_COMPILER_NO_CONSTEXPR 1
    #endif
#endif

// CC_COMPILER_NO_CONSTEXPR_IF
//
// Refers to C++17 = constexpr if(const expression) conditionals.
//
#if !defined(CC_COMPILER_NO_CONSTEXPR_IF)
    #if defined(CC_COMPILER_CPP17_ENABLED) && (defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1911))  // VS2017 15.3+
                                                                                                    // supported.
    #elif defined(CC_COMPILER_CPP17_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 309)  // Clang 3.9+
            // supported.
    #elif defined(CC_COMPILER_CPP17_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 7000)  // GCC 7+
            // supported.
    #else
        #define CC_COMPILER_NO_CONSTEXPR_IF 1
    #endif
#endif

// CC_COMPILER_NO_OVERRIDE
//
// Refers to the C++11 override specifier.
//
#ifndef CC_COMPILER_NO_OVERRIDE
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION > 1600)     // VC++ > VS2010, even without C++11 support. VS2010 does support override, however will generate warnings due to the keyword being 'non-standard'
                                                                                                    // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 209)  // Clang 2.9+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4007)  // GCC 4.7+
            // supported.
    #else
        #define CC_COMPILER_NO_OVERRIDE 1
    #endif
#endif

// CC_COMPILER_NO_INHERITANCE_FINAL
//
// Refers to the C++11 final specifier.
//
#ifndef CC_COMPILER_NO_INHERITANCE_FINAL
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1500)    // VS2008+, even without C++11 support.
                                                                                                    // supported, though you need to use CC_FINAL for it to work with VS versions prior to 2012.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 209)  // Clang 2.9+
            // supported
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4007)  // GCC 4.7+
            // supported
    #else
        #define CC_COMPILER_NO_INHERITANCE_FINAL 1
    #endif
#endif

// CC_COMPILER_NO_AUTO
//
// Refers to C++11 auto.
//
#if !defined(CC_COMPILER_NO_AUTO)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1600)      // VS2010+
                                                                                                      // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 401)  // EDG 4.1+.
            // supported with the exception of the usage of braced initializer lists as of EDG 4.3.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 209)    // Clang 2.9+, including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4004)    // GCC 4.4+
            // supported.
    #else
        #define CC_COMPILER_NO_AUTO 1
    #endif
#endif

// CC_COMPILER_NO_NULLPTR
//
// Refers to C++11 nullptr (which is a built in type). std::nullptr_t is defined in C++11 <cstddef>.
// Note that <CCore/nullptr.h> implements a portable nullptr implementation.
//
#if !defined(CC_COMPILER_NO_NULLPTR)
    #if (defined(_MSC_VER) && (_MSC_VER >= 1600)) && defined(CC_COMPILER_CPP11_ENABLED)
            // supported
    #elif defined(CC_COMPILER_GNUC) && (CC_COMPILER_VERSION >= 4006) && defined(CC_COMPILER_CPP11_ENABLED)
            // supported
    #elif defined(__clang__) && defined(CC_COMPILER_CPP11_ENABLED)
            // supported
    #elif defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 403) && defined(CC_COMPILER_CPP11_ENABLED)
            // supported
    #else
        #define CC_COMPILER_NO_NULLPTR 1
    #endif
#endif

// CC_COMPILER_NO_DECLTYPE
//
// Refers to C++11 decltype.
//
#if !defined(CC_COMPILER_NO_DECLTYPE)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1600)      // VS2010+
                                                                                                      // supported, though VS2010 doesn't support the spec completely as specified in the final standard.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 401)  // EDG 4.1+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 209)    // Clang 2.9+, including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4003)    // GCC 4.3+
            // supported.
    #else
        #define CC_COMPILER_NO_DECLTYPE 1
    #endif
#endif

// CC_COMPILER_NO_DEFAULTED_FUNCTIONS
// CC_COMPILER_NO_DELETED_FUNCTIONS
//
// Refers to C++11 = default and = delete function declarations.
//
#if !defined(CC_COMPILER_NO_DEFAULTED_FUNCTIONS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1800)      // VS2013+
                                                                                                      // supported, but as of VS2013 it isn't supported for defaulted move constructors and move assignment operators.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 401)  // EDG 4.1+.
            // supported, but as of EDG 4.3 it isn't supported for defaulted move constructors and move assignment operators until EDG 4.5.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 300)    // Clang 3.0+, including Apple's Clang
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4004)    // GCC 4.4+
            // supported.
    #else
            // VC++ doesn't support it as of VS2012.
        #define CC_COMPILER_NO_DEFAULTED_FUNCTIONS 1
    #endif
#endif

#if !defined(CC_COMPILER_NO_DELETED_FUNCTIONS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1800)      // VS2013+
                                                                                                      // supported, but as of VS2013 it isn't supported for defaulted move constructors and move assignment operators.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 401)  // EDG 4.1+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 209)    // Clang 2.9+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4004)    // GCC 4.4+
            // supported.
    #else
            // VC++ doesn't support it as of VS2012.
        #define CC_COMPILER_NO_DELETED_FUNCTIONS 1
    #endif
#endif

// CC_COMPILER_NO_LAMBDA_EXPRESSIONS
//
// Refers to C++11 lambda expressions.
//
#if !defined(CC_COMPILER_NO_LAMBDA_EXPRESSIONS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1600)      // VS2010+
                                                                                                      // supported, though VS2010 doesn't support the spec completely as specified in the final standard.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 401)  // EDG 4.1+.
            // supported. However, converting lambdas to function pointers is not supported until EDG 4.5.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 301) && !defined(__apple_build_version__)  // Clang 3.1+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4004)                                       // GCC 4.4+
            // supported.
    #else
        #define CC_COMPILER_NO_LAMBDA_EXPRESSIONS 1
    #endif
#endif

// CC_COMPILER_NO_TRAILING_RETURN_TYPES
//
// Refers to C++11 trailing-return-type. Also sometimes referred to as "incomplete return type".
//
#if !defined(CC_COMPILER_NO_TRAILING_RETURN_TYPES)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1600)      // VS2010+
                                                                                                      // supported, though VS2010 doesn't support the spec completely as specified in the final standard.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 402)  // EDG 4.2+.
            // supported. However, use of "this" in trailing return types is not supported untiil EDG 4.4
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 301) && !defined(__apple_build_version__)  // Clang 3.1+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4004)                                       // GCC 4.4+
            // supported.
    #else
        #define CC_COMPILER_NO_TRAILING_RETURN_TYPES 1
    #endif
#endif

// CC_COMPILER_NO_STRONGLY_TYPED_ENUMS
//
// Refers to C++11 strongly typed enums, which includes enum classes and sized enums. Doesn't include forward-declared enums.
//
#if !defined(CC_COMPILER_NO_STRONGLY_TYPED_ENUMS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1700)      // VS2012+
                                                                                                      // supported. A subset of this is actually supported by VS2010.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 400)  // EDG 4.0+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 209)    // Clang 2.9+, including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4004)    // GCC 4.4+
            // supported.
    #else
        #define CC_COMPILER_NO_STRONGLY_TYPED_ENUMS 1
    #endif
#endif

// CC_COMPILER_NO_FORWARD_DECLARED_ENUMS
//
// Refers to C++11 forward declared enums.
//
#if !defined(CC_COMPILER_NO_FORWARD_DECLARED_ENUMS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1700)      // VS2012+
                                                                                                      // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 405)  // EDG 4.5+.
            // supported. EDG 4.3 supports basic forward-declared enums, but not forward-declared strongly typed enums.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 301) && !defined(__apple_build_version__)  // Clang 3.1+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4006)                                       // GCC 4.6+
            // supported.
    #else
        #define CC_COMPILER_NO_FORWARD_DECLARED_ENUMS 1
    #endif
#endif

// CC_COMPILER_NO_VARIADIC_TEMPLATES
//
// Refers to C++11 variadic templates.
//
#if !defined(CC_COMPILER_NO_VARIADIC_TEMPLATES)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1800)      // VS2013+.
                                                                                                      // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (_MSC_FULL_VER == 170051025)     // VS2012 November Preview for Windows only.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 403)  // EDG 4.3+.
            // supported, though 4.1 has partial support for variadic templates.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 209)    // Clang 2.9+, including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4004)    // GCC 4.4+
            // supported, though GCC 4.3 has partial support for variadic templates.
    #else
        #define CC_COMPILER_NO_VARIADIC_TEMPLATES 1
    #endif
#endif

// CC_COMPILER_NO_TEMPLATE_ALIASES
//
// Refers to C++11 alias templates.
// Example alias template usage:
//     template <typename T>
//     using Dictionary = std::map<int, T>;
//
//     Dictionary<int> StringIntDictionary;
//
#if !defined(CC_COMPILER_NO_TEMPLATE_ALIASES)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1800)  // VS2013+.
                                                                                                  // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 402)                                     // EDG 4.2+.
            // supported, though 4.1 has partial support for variadic templates.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 300) && !defined(__apple_build_version__)  // Clang 3.0+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4007)                                       // GCC 4.7+
            // supported, though GCC 4.3 has partial support for variadic templates.
    #else
        #define CC_COMPILER_NO_TEMPLATE_ALIASES 1
    #endif
#endif

// CC_COMPILER_NO_VARIABLE_TEMPLATES
//
// Refers to C++14 variable templates.
// Example variable template usage:
//     template<class T>
//     constexpr T pi = T(3.1415926535897932385);
//
#if !defined(CC_COMPILER_NO_VARIABLE_TEMPLATES)
    #if defined(_MSC_VER) && (_MSC_FULL_VER >= 190023918)                                                                                // VS2015 Update 2 and above.
                                                                                                                                         // supported.
    #elif defined(CC_COMPILER_CPP14_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 304) && !defined(__apple_build_version__)  // Clang 3.4+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP14_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 5000)                                       // GCC 5+
            // supported.
    #elif !defined(CC_COMPILER_CPP14_ENABLED)
        #define CC_COMPILER_NO_VARIABLE_TEMPLATES 1
    #endif
#endif

// CC_COMPILER_NO_INLINE_VARIABLES
//
// Refers to C++17 inline variables that allows the definition of variables in header files
//
// Example usage:
//    struct Foo
//    {
//        static inline constexpr int kConstant = 42;  // no out of class definition
//    };
//
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4424.pdf
// http://en.cppreference.com/w/cpp/language/inline
//
#if !defined(CC_COMPILER_NO_INLINE_VARIABLES)
    #define CC_COMPILER_NO_INLINE_VARIABLES 1
#endif

// CC_COMPILER_NO_INITIALIZER_LISTS
//
// Refers to C++11 initializer lists.
// This refers to the compiler support for this and not the Standard Library support (std::initializer_list).
//
#if !defined(CC_COMPILER_NO_INITIALIZER_LISTS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1800)      // VS2013+.
                                                                                                      // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (_MSC_FULL_VER == 170051025)     // VS2012 November Preview for Windows only.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 405)  // EDG 4.5+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 301) && !defined(__apple_build_version__)  // Clang 3.1+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4004)                                       // GCC 4.4+
            // supported, though GCC 4.3 has partial support for it.
    #else
        #define CC_COMPILER_NO_INITIALIZER_LISTS 1
    #endif
#endif

// CC_COMPILER_NO_NORETURN
//
// Refers to C++11 declaration attribute: noreturn.
// http://en.cppreference.com/w/cpp/language/attributes
// http://blog.aaronballman.com/2011/09/understanding-attributes/
//
#if !defined(CC_COMPILER_NO_NORETURN)
    #if defined(CC_COMPILER_MSVC) && (CC_COMPILER_VERSION >= 1300)                                    // VS2003+
                                                                                                      // supported via __declspec(noreturn). You need to use that or CC_NORETURN. VC++ up to VS2013 doesn't support any C++11 attribute types.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 402)  // EDG 4.2+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 300) && !defined(__apple_build_version__)  // Clang 3.0+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4008)                                       // GCC 4.8+
            // supported.
    #else
        #define CC_COMPILER_NO_NORETURN 1
    #endif
#endif

// CC_COMPILER_NO_CARRIES_DEPENDENCY
//
// Refers to C++11 declaration attribute: carries_dependency.
// http://en.cppreference.com/w/cpp/language/attributes
// http://blog.aaronballman.com/2011/09/understanding-attributes/
//
#if !defined(CC_COMPILER_NO_CARRIES_DEPENDENCY)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)     // Apple clang 4.1+
                                                                                                                                         // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 402)                                     // EDG 4.2+.
            // supported; stricter than other compilers in its usage.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 300) && !defined(__apple_build_version__)  // Clang 3.0+, not including Apple's Clang.
            // supported.
    // Currently GNUC doesn't appear to support this attribute.
    // #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4008)                        // GCC 4.8+
    //    // supported.
    #else
        #define CC_COMPILER_NO_CARRIES_DEPENDENCY 1
    #endif
#endif

// CC_COMPILER_NO_FALLTHROUGH
//
// Refers to C++17 declaration attribute: fallthrough.
// http://en.cppreference.com/w/cpp/language/attributes
//
#if !defined(CC_COMPILER_NO_FALLTHROUGH)
    #if defined(CC_COMPILER_CPP17_ENABLED)
            // supported.
    #else
        #define CC_COMPILER_NO_FALLTHROUGH 1
    #endif
#endif

// CC_COMPILER_NO_NODISCARD
//
// Refers to C++17 declaration attribute: nodiscard.
// http://en.cppreference.com/w/cpp/language/attributes
//
#if !defined(CC_COMPILER_NO_NODISCARD)
    #if defined(CC_COMPILER_CPP17_ENABLED)
            // supported.
    #else
        #define CC_COMPILER_NO_NODISCARD 1
    #endif
#endif

// CC_COMPILER_NO_MAYBE_UNUSED
//
// Refers to C++17 declaration attribute: maybe_unused.
// http://en.cppreference.com/w/cpp/language/attributes
//
#if !defined(CC_COMPILER_NO_MAYBE_UNUSED)
    #if defined(CC_COMPILER_CPP17_ENABLED)
            // supported.
    #elif defined(CC_COMPILER_MSVC) && (CC_COMPILER_VERSION >= 1912)  // VS2017 15.3+
            // supported.
    #else
        #define CC_COMPILER_NO_MAYBE_UNUSED 1
    #endif
#endif

// CC_COMPILER_NO_STRUCTURED_BINDING
//
// Indicates if target compiler supports the C++17 "structured binding" language feature.
// https://en.cppreference.com/w/cpp/language/structured_binding
//
//
#if !defined(CC_COMPILER_NO_STRUCTURED_BINDING)
    #if defined(CC_COMPILER_CPP17_ENABLED)
            // supported.
    #elif defined(CC_COMPILER_MSVC) && (CC_COMPILER_VERSION >= 1912)  // VS2017 15.3+
            // supported.
    #else
        #define CC_COMPILER_NO_STRUCTURED_BINDING 1
    #endif
#endif

// CC_COMPILER_NO_DESIGNATED_INITIALIZERS
//
// Indicates the target compiler supports the C++20 "designated initializer" language feature.
// https://en.cppreference.com/w/cpp/language/aggregate_initialization
//
// Example:
//   struct A { int x; int y; };
//   A a = { .y = 42, .x = 1 };
//
#if !defined(CC_COMPILER_NO_DESIGNATED_INITIALIZERS)
    #if defined(CC_COMPILER_CPP20_ENABLED)
            // supported.
    #else
        #define CC_COMPILER_NO_DESIGNATED_INITIALIZERS 1
    #endif
#endif

// CC_COMPILER_NO_NONSTATIC_MEMBER_INITIALIZERS
//
// Refers to C++11 declaration attribute: carries_dependency.
// http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2008/n2756.htm
//
#if !defined(CC_COMPILER_NO_NONSTATIC_MEMBER_INITIALIZERS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1800)                                         // VS2013+.
                                                                                                                                         // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)   // Apple clang 4.1+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 300) && !defined(__apple_build_version__)  // Clang 3.0+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4007)                                       // GCC 4.7+
            // supported.
    #else
        #define CC_COMPILER_NO_NONSTATIC_MEMBER_INITIALIZERS 1
    #endif
#endif

// CC_COMPILER_NO_RIGHT_ANGLE_BRACKETS
//
// Defines if the compiler supports >> (as opposed to > >) in template
// declarations such as typedef eastl::list<eastl::list<int>> ListList;
//
#if !defined(CC_COMPILER_NO_RIGHT_ANGLE_BRACKETS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1600)      // VS2010+
                                                                                                      // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 401)  // EDG 4.1+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 209)    // Clang 2.9+, including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4003)    // GCC 4.3+
            // supported.
    #else
        #define CC_COMPILER_NO_RIGHT_ANGLE_BRACKETS 1
    #endif
#endif

// CC_COMPILER_NO_ALIGNOF
//
// Refers specifically to C++11 alignof and not old compiler extensions such as __alignof__().
// However, CCore provides a portable CC_ALIGN_OF which works for all compilers.
//
#if !defined(CC_COMPILER_NO_ALIGNOF)
    // Not supported by VC++ as of VS2013, though CC_ALIGN_OF is supported on all coompilers as an alternative.
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 209)    // Clang 2.9+, including Apple's Clang.
                                                                                                    // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4005)  // GCC 4.5+
            // supported.
    #else
        #define CC_COMPILER_NO_ALIGNOF 1
    #endif
#endif

// CC_COMPILER_NO_ALIGNAS
//
// Refers to C++11 alignas.
//
#if !defined(CC_COMPILER_NO_ALIGNAS)
    // Not supported by VC++ as of VS2013.
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)     // Apple clang 4.1+
                                                                                                                                         // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 300) && !defined(__apple_build_version__)  // Clang 3.0+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4008)                                       // GCC 4.8+
            // supported.
    #else
        #define CC_COMPILER_NO_ALIGNAS 1
    #endif
#endif

// CC_COMPILER_NO_DELEGATING_CONSTRUCTORS
//
// Refers to C++11 constructor delegation.
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1986.pdf
// https://www.ibm.com/developerworks/mydeveloperworks/blogs/5894415f-be62-4bc0-81c5-3956e82276f3/entry/c_0x_delegating_constructors
//
#if !defined(CC_COMPILER_NO_DELEGATING_CONSTRUCTORS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1800)                                         // VS2013+.
                                                                                                                                         // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 407)                                     // EDG 4.7+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)   // Apple clang 4.1+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 300) && !defined(__apple_build_version__)  // Clang 3.0+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4007)                                       // GCC 4.7+
            // supported.
    #else
        #define CC_COMPILER_NO_DELEGATING_CONSTRUCTORS 1
    #endif
#endif

// CC_COMPILER_NO_INHERITING_CONSTRUCTORS
//
// Refers to C++11 constructor inheritance via 'using'.
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2540.htm
//
#if !defined(CC_COMPILER_NO_INHERITING_CONSTRUCTORS)
    // Not supported by VC++ as of VS2013.
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && CC_COMPILER_HAS_FEATURE(cxx_inheriting_constructors)  // Clang
                                                                                                                          // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4008)                        // GCC 4.8+
            // supported.
    #else
        #define CC_COMPILER_NO_INHERITING_CONSTRUCTORS 1
    #endif
#endif

// CC_COMPILER_NO_USER_DEFINED_LITERALS
//
// http://en.cppreference.com/w/cpp/language/user_literal
// http://stackoverflow.com/questions/237804/what-new-capabilities-do-user-defined-literals-add-to-c
//
#if !defined(CC_COMPILER_NO_USER_DEFINED_LITERALS)
    // Not supported by VC++ as of VS2013.
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)     // Apple clang 4.1+
                                                                                                                                         // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 301) && !defined(__apple_build_version__)  // Clang 3.1+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4007)                                       // GCC 4.7+
            // supported.
    #else
        #define CC_COMPILER_NO_USER_DEFINED_LITERALS 1
    #endif
#endif

// CC_COMPILER_NO_STANDARD_LAYOUT_TYPES
//     a.k.a. POD relaxation
//     http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2342.htm
//
#if !defined(CC_COMPILER_NO_STANDARD_LAYOUT_TYPES)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1700)                                         // VS2012+
                                                                                                                                         // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)   // Apple clang 4.1+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 300) && !defined(__apple_build_version__)  // Clang 3.0+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4005)                                       // GCC 4.5+
            // supported.
    #else
        #define CC_COMPILER_NO_STANDARD_LAYOUT_TYPES 1
    #endif
#endif

// CC_COMPILER_NO_EXTENDED_SIZEOF
//
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2253.html
// Allows you to do this: sizeof(SomeClass::mSomeMember)
//
#if !defined(CC_COMPILER_NO_EXTENDED_SIZEOF)
    // Not supported by VC++ as of VS2013.
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)  // Apple clang 4.1+
                                                                                                                                      // supported.
    // Versions of EDG prior to 4.5 only support extended sizeof in non-member functions. Full support was added in 4.5
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 405)                                     // EDG 4.5+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 301) && !defined(__apple_build_version__)  // Clang 3.1+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4005)                                       // GCC 4.5+
            // supported.
    #else
        #define CC_COMPILER_NO_EXTENDED_SIZEOF 1
    #endif
#endif

// CC_COMPILER_NO_INLINE_NAMESPACES
//
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2535.htm
// http://blog.aaronballman.com/2011/07/inline-namespaces/
//
#if !defined(CC_COMPILER_NO_INLINE_NAMESPACES)
    // Not supported by VC++ as of VS2013.
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 405)  // EDG 4.5+.
                                                                                                    // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 209)  // Clang 2.9+, including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4004)  // GCC 4.4+
            // supported.
    #else
        #define CC_COMPILER_NO_INLINE_NAMESPACES 1
    #endif
#endif

// CC_COMPILER_NO_UNRESTRICTED_UNIONS
//
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2544.pdf
//
#if !defined(CC_COMPILER_NO_UNRESTRICTED_UNIONS)
    // Not supported by VC++ as of VS2013.
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 406)                                       // EDG 4.6+.
                                                                                                                                         // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)   // Apple clang 4.1+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 301) && !defined(__apple_build_version__)  // Clang 3.1+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4006)                                       // GCC 4.6+
            // supported.
    #else
        #define CC_COMPILER_NO_UNRESTRICTED_UNIONS 1
    #endif
#endif

// CC_COMPILER_NO_EXPLICIT_CONVERSION_OPERATORS
//
// http://en.wikipedia.org/wiki/C%2B%2B11#Explicit_conversion_operators
//
#if !defined(CC_COMPILER_NO_EXPLICIT_CONVERSION_OPERATORS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1800)                                         // VS2013+.
                                                                                                                                         // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (_MSC_FULL_VER == 170051025)                                        // VS2012 November Preview for Windows only.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 404)                                     // EDG 4.4+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)   // Apple clang 4.1+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 300) && !defined(__apple_build_version__)  // Clang 3.0+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4005)                                       // GCC 4.5+
            // supported.
    #else
        #define CC_COMPILER_NO_EXPLICIT_CONVERSION_OPERATORS 1
    #endif
#endif

// CC_COMPILER_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS
//
// The compiler does not support default template arguments for function templates.
// http://stackoverflow.com/questions/2447458/default-template-arguments-for-function-templates
//
#if !defined(CC_COMPILER_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1800)      // VS2013+.
                                                                                                      // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 403)  // EDG 4.4+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 209)    // Clang 2.9+, including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4003)    // GCC 4.3+
            // supported.
    #else
        #define CC_COMPILER_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS 1
    #endif
#endif

// CC_COMPILER_NO_LOCAL_CLASS_TEMPLATE_PARAMETERS
//
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2657.htm
// http://stackoverflow.com/questions/5751977/local-type-as-template-arguments-in-c
//
#if !defined(CC_COMPILER_NO_LOCAL_CLASS_TEMPLATE_PARAMETERS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1600)  // VS2010+
        // supported.
        #if (CC_COMPILER_VERSION < 1700)     // VS2010 generates a warning, but the C++ language now allows it.
            #pragma warning(disable : 4836)  // nonstandard extension used: local types or unnamed types cannot be used as template arguments.
        #endif
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 402)  // EDG 4.2+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 209)    // Clang 2.9+, including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4005)    // GCC 4.5+
            // supported.
    #else
        #define CC_COMPILER_NO_LOCAL_CLASS_TEMPLATE_PARAMETERS 1
    #endif
#endif

// CC_COMPILER_NO_NOEXCEPT
//
// C++11 noexcept
// http://en.cppreference.com/w/cpp/language/attributes
// http://en.cppreference.com/w/cpp/language/noexcept
//
#if !defined(CC_COMPILER_NO_NOEXCEPT)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1900)                                         // VS2014+
                                                                                                                                         // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)   // Apple clang 4.1+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 405)                                     // EDG 4.5+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 300) && !defined(__apple_build_version__)  // Clang 3.0+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4006)                                       // GCC 4.6+
            // supported.
    #else
        #define CC_COMPILER_NO_NOEXCEPT 1
    #endif
#endif

// CC_COMPILER_NO_RAW_LITERALS
//
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2442.htm
// http://en.wikipedia.org/wiki/C%2B%2B11#New_string_literals
//
#if !defined(CC_COMPILER_NO_RAW_LITERALS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1800)                                         // VS2013+.
                                                                                                                                         // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 407)                                     // EDG 4.7+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)   // Apple clang 4.1+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 300) && !defined(__apple_build_version__)  // Clang 3.0+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4005)                                       // GCC 4.5+
            // supported.
    #else
        #define CC_COMPILER_NO_RAW_LITERALS 1
    #endif
#endif

// CC_COMPILER_NO_UNICODE_STRING_LITERALS
//
// http://en.wikipedia.org/wiki/C%2B%2B11#New_string_literals
//
#if !defined(CC_COMPILER_NO_UNICODE_STRING_LITERALS)
    // Not supported by VC++ as of VS2013.
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 407)                                       // EDG 4.7+.
                                                                                                                                         // supported. It's not clear if it's v4.4 or v4.7 that adds this support.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)   // Apple clang 4.1+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 300) && !defined(__apple_build_version__)  // Clang 3.0+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4004)                                       // GCC 4.4+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 407)                                     // EDG 4.7+.
            // supported. It's not clear if it's v4.4 or v4.7 that adds this support.
    #else
        #define CC_COMPILER_NO_UNICODE_STRING_LITERALS 1
    #endif
#endif

// CC_COMPILER_NO_NEW_CHARACTER_TYPES
//
// Refers to char16_t and char32_t as true native types (and not something simply typedef'd from uint16_t and uint32_t).
// http://en.cppreference.com/w/cpp/language/types
//
#if !defined(CC_COMPILER_NO_NEW_CHARACTER_TYPES)
    #if defined(CC_COMPILER_NO_UNICODE_STRING_LITERALS)  // Some compilers have had support for char16_t prior to support for u"", but it's not useful to have the former without the latter.
        #define CC_COMPILER_NO_NEW_CHARACTER_TYPES 1
    #endif
#endif

// CC_COMPILER_NO_UNICODE_CHAR_NAME_LITERALS
//
// C++ 11 relaxed \u\U sequences in strings.
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2170.html
//
#if !defined(CC_COMPILER_NO_UNICODE_CHAR_NAME_LITERALS)
        // VC++ up till at least VS2013 supports \u and \U but supports them wrong with respect to the C++11 Standard.

    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)     // Apple clang 4.1+
                                                                                                                                         // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 301) && !defined(__apple_build_version__)  // Clang 3.1+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4005)                                       // GCC 4.5+
            // supported.
    #else
        #define CC_COMPILER_NO_UNICODE_CHAR_NAME_LITERALS 1
    #endif
#endif

// CC_COMPILER_NO_UNIFIED_INITIALIZATION_SYNTAX
//
// http://en.wikipedia.org/wiki/C%2B%2B11#Uniform_initialization
//
#if !defined(CC_COMPILER_NO_UNIFIED_INITIALIZATION_SYNTAX)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1800)                                         // VS2013+.
                                                                                                                                         // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 401) && defined(__apple_build_version__)   // Apple clang 4.1+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 301) && !defined(__apple_build_version__)  // Clang 3.1+, not including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4004)                                       // GCC 4.4+
            // supported.
    #else
        #define CC_COMPILER_NO_UNIFIED_INITIALIZATION_SYNTAX 1
    #endif
#endif

// CC_COMPILER_NO_EXTENDED_FRIEND_DECLARATIONS
//
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1791.pdf
//
#if !defined(CC_COMPILER_NO_EXTENDED_FRIEND_DECLARATIONS)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1600)      // VS2010+
                                                                                                      // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 401)  // EDG 4.1+.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && (CC_COMPILER_VERSION >= 209)    // Clang 2.9+, including Apple's Clang.
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4007)    // GCC 4.7+
            // supported.
    #else
        #define CC_COMPILER_NO_EXTENDED_FRIEND_DECLARATIONS 1
    #endif
#endif

// CC_COMPILER_NO_THREAD_LOCAL
//
// Refers specifically to C++ thread_local, which is like compiler __thread implementations except
// that it also supports non-trivial classes (e.g. with ctors). CC_COMPILER_NO_THREAD_LOCAL refers
// specifically to full C++11 thread_local support. The EAThread package provides a wrapper for
// __thread via CC_THREAD_LOCAL (which unfortunately sounds like C++ thread_local).
//
// https://en.cppreference.com/w/cpp/keyword/thread_local
//
#if !defined(CC_COMPILER_NO_THREAD_LOCAL)
    #if defined(CC_COMPILER_CPP11_ENABLED) && defined(__clang__) && CC_COMPILER_HAS_FEATURE(cxx_thread_local)
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(_MSC_VER) && (CC_COMPILER_VERSION >= 1900)  // VS2015+
            // supported.
    #elif defined(CC_COMPILER_CPP11_ENABLED) && defined(__GNUC__) && (CC_COMPILER_VERSION >= 4008)  // GCC 4.8+
            // supported.
    #else
        #define CC_COMPILER_NO_THREAD_LOCAL 1
    #endif
#endif

};  // namespace ncore

#endif
