
// This is a generated file. Do not edit!

#ifndef ANNOTATOR_COMPILER_DETECTION_H
#define ANNOTATOR_COMPILER_DETECTION_H

#ifdef __cplusplus
#define ANNOTATOR_COMPILER_IS_Comeau 0
#define ANNOTATOR_COMPILER_IS_Intel 0
#define ANNOTATOR_COMPILER_IS_PathScale 0
#define ANNOTATOR_COMPILER_IS_Embarcadero 0
#define ANNOTATOR_COMPILER_IS_Borland 0
#define ANNOTATOR_COMPILER_IS_Watcom 0
#define ANNOTATOR_COMPILER_IS_OpenWatcom 0
#define ANNOTATOR_COMPILER_IS_SunPro 0
#define ANNOTATOR_COMPILER_IS_HP 0
#define ANNOTATOR_COMPILER_IS_Compaq 0
#define ANNOTATOR_COMPILER_IS_zOS 0
#define ANNOTATOR_COMPILER_IS_XL 0
#define ANNOTATOR_COMPILER_IS_VisualAge 0
#define ANNOTATOR_COMPILER_IS_PGI 0
#define ANNOTATOR_COMPILER_IS_Cray 0
#define ANNOTATOR_COMPILER_IS_TI 0
#define ANNOTATOR_COMPILER_IS_Fujitsu 0
#define ANNOTATOR_COMPILER_IS_SCO 0
#define ANNOTATOR_COMPILER_IS_AppleClang 0
#define ANNOTATOR_COMPILER_IS_Clang 0
#define ANNOTATOR_COMPILER_IS_GNU 0
#define ANNOTATOR_COMPILER_IS_MSVC 0
#define ANNOTATOR_COMPILER_IS_ADSP 0
#define ANNOTATOR_COMPILER_IS_IAR 0
#define ANNOTATOR_COMPILER_IS_MIPSpro 0

#if defined(__COMO__)
#undef ANNOTATOR_COMPILER_IS_Comeau
#define ANNOTATOR_COMPILER_IS_Comeau 1

#elif defined(__INTEL_COMPILER) || defined(__ICC)
#undef ANNOTATOR_COMPILER_IS_Intel
#define ANNOTATOR_COMPILER_IS_Intel 1

#elif defined(__PATHCC__)
#undef ANNOTATOR_COMPILER_IS_PathScale
#define ANNOTATOR_COMPILER_IS_PathScale 1

#elif defined(__BORLANDC__) && defined(__CODEGEARC_VERSION__)
#undef ANNOTATOR_COMPILER_IS_Embarcadero
#define ANNOTATOR_COMPILER_IS_Embarcadero 1

#elif defined(__BORLANDC__)
#undef ANNOTATOR_COMPILER_IS_Borland
#define ANNOTATOR_COMPILER_IS_Borland 1

#elif defined(__WATCOMC__) && __WATCOMC__ < 1200
#undef ANNOTATOR_COMPILER_IS_Watcom
#define ANNOTATOR_COMPILER_IS_Watcom 1

#elif defined(__WATCOMC__)
#undef ANNOTATOR_COMPILER_IS_OpenWatcom
#define ANNOTATOR_COMPILER_IS_OpenWatcom 1

#elif defined(__SUNPRO_CC)
#undef ANNOTATOR_COMPILER_IS_SunPro
#define ANNOTATOR_COMPILER_IS_SunPro 1

#elif defined(__HP_aCC)
#undef ANNOTATOR_COMPILER_IS_HP
#define ANNOTATOR_COMPILER_IS_HP 1

#elif defined(__DECCXX)
#undef ANNOTATOR_COMPILER_IS_Compaq
#define ANNOTATOR_COMPILER_IS_Compaq 1

#elif defined(__IBMCPP__) && defined(__COMPILER_VER__)
#undef ANNOTATOR_COMPILER_IS_zOS
#define ANNOTATOR_COMPILER_IS_zOS 1

#elif defined(__IBMCPP__) && !defined(__COMPILER_VER__) && __IBMCPP__ >= 800
#undef ANNOTATOR_COMPILER_IS_XL
#define ANNOTATOR_COMPILER_IS_XL 1

#elif defined(__IBMCPP__) && !defined(__COMPILER_VER__) && __IBMCPP__ < 800
#undef ANNOTATOR_COMPILER_IS_VisualAge
#define ANNOTATOR_COMPILER_IS_VisualAge 1

#elif defined(__PGI)
#undef ANNOTATOR_COMPILER_IS_PGI
#define ANNOTATOR_COMPILER_IS_PGI 1

#elif defined(_CRAYC)
#undef ANNOTATOR_COMPILER_IS_Cray
#define ANNOTATOR_COMPILER_IS_Cray 1

#elif defined(__TI_COMPILER_VERSION__)
#undef ANNOTATOR_COMPILER_IS_TI
#define ANNOTATOR_COMPILER_IS_TI 1

#elif defined(__FUJITSU) || defined(__FCC_VERSION) || defined(__fcc_version)
#undef ANNOTATOR_COMPILER_IS_Fujitsu
#define ANNOTATOR_COMPILER_IS_Fujitsu 1

#elif defined(__SCO_VERSION__)
#undef ANNOTATOR_COMPILER_IS_SCO
#define ANNOTATOR_COMPILER_IS_SCO 1

#elif defined(__clang__) && defined(__apple_build_version__)
#undef ANNOTATOR_COMPILER_IS_AppleClang
#define ANNOTATOR_COMPILER_IS_AppleClang 1

#elif defined(__clang__)
#undef ANNOTATOR_COMPILER_IS_Clang
#define ANNOTATOR_COMPILER_IS_Clang 1

#elif defined(__GNUC__)
#undef ANNOTATOR_COMPILER_IS_GNU
#define ANNOTATOR_COMPILER_IS_GNU 1

#elif defined(_MSC_VER)
#undef ANNOTATOR_COMPILER_IS_MSVC
#define ANNOTATOR_COMPILER_IS_MSVC 1

#elif defined(__VISUALDSPVERSION__) || defined(__ADSPBLACKFIN__) || \
    defined(__ADSPTS__) || defined(__ADSP21000__)
#undef ANNOTATOR_COMPILER_IS_ADSP
#define ANNOTATOR_COMPILER_IS_ADSP 1

#elif defined(__IAR_SYSTEMS_ICC__) || defined(__IAR_SYSTEMS_ICC)
#undef ANNOTATOR_COMPILER_IS_IAR
#define ANNOTATOR_COMPILER_IS_IAR 1

#elif defined(_SGI_COMPILER_VERSION) || defined(_COMPILER_VERSION)
#undef ANNOTATOR_COMPILER_IS_MIPSpro
#define ANNOTATOR_COMPILER_IS_MIPSpro 1

#endif

#if ANNOTATOR_COMPILER_IS_AppleClang

#if !(((__clang_major__ * 100) + __clang_minor__) >= 400)
#error Unsupported compiler version
#endif

#define ANNOTATOR_COMPILER_VERSION_MAJOR (__clang_major__)
#define ANNOTATOR_COMPILER_VERSION_MINOR (__clang_minor__)
#define ANNOTATOR_COMPILER_VERSION_PATCH (__clang_patchlevel__)
#if defined(_MSC_VER)
/* _MSC_VER = VVRR */
#define ANNOTATOR_SIMULATE_VERSION_MAJOR (_MSC_VER / 100)
#define ANNOTATOR_SIMULATE_VERSION_MINOR (_MSC_VER % 100)
#endif
#define ANNOTATOR_COMPILER_VERSION_TWEAK (__apple_build_version__)

#if ((__clang_major__ * 100) + __clang_minor__) >= 400 && \
    __has_feature(cxx_alignas)
#define ANNOTATOR_COMPILER_CXX_ALIGNAS 1
#else
#define ANNOTATOR_COMPILER_CXX_ALIGNAS 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 400 && \
    __has_feature(cxx_alignas)
#define ANNOTATOR_COMPILER_CXX_ALIGNOF 1
#else
#define ANNOTATOR_COMPILER_CXX_ALIGNOF 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 400 && \
    __has_feature(cxx_constexpr)
#define ANNOTATOR_COMPILER_CXX_CONSTEXPR 1
#else
#define ANNOTATOR_COMPILER_CXX_CONSTEXPR 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 400 && \
    __has_feature(cxx_override_control)
#define ANNOTATOR_COMPILER_CXX_FINAL 1
#else
#define ANNOTATOR_COMPILER_CXX_FINAL 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 400 && \
    __has_feature(cxx_noexcept)
#define ANNOTATOR_COMPILER_CXX_NOEXCEPT 1
#else
#define ANNOTATOR_COMPILER_CXX_NOEXCEPT 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 400 && \
    __has_feature(cxx_nullptr)
#define ANNOTATOR_COMPILER_CXX_NULLPTR 1
#else
#define ANNOTATOR_COMPILER_CXX_NULLPTR 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 400 && __cplusplus >= 201103L
#define ANNOTATOR_COMPILER_CXX_SIZEOF_MEMBER 1
#else
#define ANNOTATOR_COMPILER_CXX_SIZEOF_MEMBER 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 400 && \
    __has_feature(cxx_thread_local)
#define ANNOTATOR_COMPILER_CXX_THREAD_LOCAL 1
#else
#define ANNOTATOR_COMPILER_CXX_THREAD_LOCAL 0
#endif

#elif ANNOTATOR_COMPILER_IS_Clang

#if !(((__clang_major__ * 100) + __clang_minor__) >= 304)
#error Unsupported compiler version
#endif

#define ANNOTATOR_COMPILER_VERSION_MAJOR (__clang_major__)
#define ANNOTATOR_COMPILER_VERSION_MINOR (__clang_minor__)
#define ANNOTATOR_COMPILER_VERSION_PATCH (__clang_patchlevel__)
#if defined(_MSC_VER)
/* _MSC_VER = VVRR */
#define ANNOTATOR_SIMULATE_VERSION_MAJOR (_MSC_VER / 100)
#define ANNOTATOR_SIMULATE_VERSION_MINOR (_MSC_VER % 100)
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 304 && \
    __has_feature(cxx_alignas)
#define ANNOTATOR_COMPILER_CXX_ALIGNAS 1
#else
#define ANNOTATOR_COMPILER_CXX_ALIGNAS 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 304 && \
    __has_feature(cxx_alignas)
#define ANNOTATOR_COMPILER_CXX_ALIGNOF 1
#else
#define ANNOTATOR_COMPILER_CXX_ALIGNOF 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 304 && \
    __has_feature(cxx_constexpr)
#define ANNOTATOR_COMPILER_CXX_CONSTEXPR 1
#else
#define ANNOTATOR_COMPILER_CXX_CONSTEXPR 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 304 && \
    __has_feature(cxx_override_control)
#define ANNOTATOR_COMPILER_CXX_FINAL 1
#else
#define ANNOTATOR_COMPILER_CXX_FINAL 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 304 && \
    __has_feature(cxx_noexcept)
#define ANNOTATOR_COMPILER_CXX_NOEXCEPT 1
#else
#define ANNOTATOR_COMPILER_CXX_NOEXCEPT 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 304 && \
    __has_feature(cxx_nullptr)
#define ANNOTATOR_COMPILER_CXX_NULLPTR 1
#else
#define ANNOTATOR_COMPILER_CXX_NULLPTR 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 304 && __cplusplus >= 201103L
#define ANNOTATOR_COMPILER_CXX_SIZEOF_MEMBER 1
#else
#define ANNOTATOR_COMPILER_CXX_SIZEOF_MEMBER 0
#endif

#if ((__clang_major__ * 100) + __clang_minor__) >= 304 && \
    __has_feature(cxx_thread_local)
#define ANNOTATOR_COMPILER_CXX_THREAD_LOCAL 1
#else
#define ANNOTATOR_COMPILER_CXX_THREAD_LOCAL 0
#endif

#elif ANNOTATOR_COMPILER_IS_GNU

#if !((__GNUC__ * 100 + __GNUC_MINOR__) >= 404)
#error Unsupported compiler version
#endif

#define ANNOTATOR_COMPILER_VERSION_MAJOR (__GNUC__)
#define ANNOTATOR_COMPILER_VERSION_MINOR (__GNUC_MINOR__)
#if defined(__GNUC_PATCHLEVEL__)
#define ANNOTATOR_COMPILER_VERSION_PATCH (__GNUC_PATCHLEVEL__)
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 408 && __cplusplus >= 201103L
#define ANNOTATOR_COMPILER_CXX_ALIGNAS 1
#else
#define ANNOTATOR_COMPILER_CXX_ALIGNAS 0
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 408 && __cplusplus >= 201103L
#define ANNOTATOR_COMPILER_CXX_ALIGNOF 1
#else
#define ANNOTATOR_COMPILER_CXX_ALIGNOF 0
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 406 && \
    (__cplusplus >= 201103L ||                  \
     (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GXX_EXPERIMENTAL_CXX0X__))
#define ANNOTATOR_COMPILER_CXX_CONSTEXPR 1
#else
#define ANNOTATOR_COMPILER_CXX_CONSTEXPR 0
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 407 && __cplusplus >= 201103L
#define ANNOTATOR_COMPILER_CXX_FINAL 1
#else
#define ANNOTATOR_COMPILER_CXX_FINAL 0
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 406 && \
    (__cplusplus >= 201103L ||                  \
     (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GXX_EXPERIMENTAL_CXX0X__))
#define ANNOTATOR_COMPILER_CXX_NOEXCEPT 1
#else
#define ANNOTATOR_COMPILER_CXX_NOEXCEPT 0
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 406 && \
    (__cplusplus >= 201103L ||                  \
     (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GXX_EXPERIMENTAL_CXX0X__))
#define ANNOTATOR_COMPILER_CXX_NULLPTR 1
#else
#define ANNOTATOR_COMPILER_CXX_NULLPTR 0
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 404 && \
    (__cplusplus >= 201103L ||                  \
     (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GXX_EXPERIMENTAL_CXX0X__))
#define ANNOTATOR_COMPILER_CXX_SIZEOF_MEMBER 1
#else
#define ANNOTATOR_COMPILER_CXX_SIZEOF_MEMBER 0
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 408 && __cplusplus >= 201103L
#define ANNOTATOR_COMPILER_CXX_THREAD_LOCAL 1
#else
#define ANNOTATOR_COMPILER_CXX_THREAD_LOCAL 0
#endif

#elif ANNOTATOR_COMPILER_IS_MSVC

#if !(_MSC_VER >= 1600)
#error Unsupported compiler version
#endif

/* _MSC_VER = VVRR */
#define ANNOTATOR_COMPILER_VERSION_MAJOR (_MSC_VER / 100)
#define ANNOTATOR_COMPILER_VERSION_MINOR (_MSC_VER % 100)
#if defined(_MSC_FULL_VER)
#if _MSC_VER >= 1400
/* _MSC_FULL_VER = VVRRPPPPP */
#define ANNOTATOR_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 100000)
#else
/* _MSC_FULL_VER = VVRRPPPP */
#define ANNOTATOR_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 10000)
#endif
#endif
#if defined(_MSC_BUILD)
#define ANNOTATOR_COMPILER_VERSION_TWEAK (_MSC_BUILD)
#endif

#if _MSC_VER >= 1900
#define ANNOTATOR_COMPILER_CXX_ALIGNAS 1
#else
#define ANNOTATOR_COMPILER_CXX_ALIGNAS 0
#endif

#if _MSC_VER >= 1900
#define ANNOTATOR_COMPILER_CXX_ALIGNOF 1
#else
#define ANNOTATOR_COMPILER_CXX_ALIGNOF 0
#endif

#define ANNOTATOR_COMPILER_CXX_CONSTEXPR 0

#if _MSC_VER >= 1700
#define ANNOTATOR_COMPILER_CXX_FINAL 1
#else
#define ANNOTATOR_COMPILER_CXX_FINAL 0
#endif

#if _MSC_VER >= 1900
#define ANNOTATOR_COMPILER_CXX_NOEXCEPT 1
#else
#define ANNOTATOR_COMPILER_CXX_NOEXCEPT 0
#endif

#if _MSC_VER >= 1600
#define ANNOTATOR_COMPILER_CXX_NULLPTR 1
#else
#define ANNOTATOR_COMPILER_CXX_NULLPTR 0
#endif

#if _MSC_VER >= 1900
#define ANNOTATOR_COMPILER_CXX_SIZEOF_MEMBER 1
#else
#define ANNOTATOR_COMPILER_CXX_SIZEOF_MEMBER 0
#endif

#if _MSC_VER >= 1900
#define ANNOTATOR_COMPILER_CXX_THREAD_LOCAL 1
#else
#define ANNOTATOR_COMPILER_CXX_THREAD_LOCAL 0
#endif

#else
#error Unsupported compiler
#endif

#if ANNOTATOR_COMPILER_CXX_ALIGNAS
#define ANNOTATOR_ALIGNAS(X) alignas(X)
#elif ANNOTATOR_COMPILER_IS_GNU || ANNOTATOR_COMPILER_IS_Clang || \
    ANNOTATOR_COMPILER_IS_AppleClang
#define ANNOTATOR_ALIGNAS(X) __attribute__((__aligned__(X)))
#elif ANNOTATOR_COMPILER_IS_MSVC
#define ANNOTATOR_ALIGNAS(X) __declspec(align(X))
#else
#define ANNOTATOR_ALIGNAS(X)
#endif

#if ANNOTATOR_COMPILER_CXX_ALIGNOF
#define ANNOTATOR_ALIGNOF(X) alignof(X)
#elif ANNOTATOR_COMPILER_IS_GNU || ANNOTATOR_COMPILER_IS_Clang || \
    ANNOTATOR_COMPILER_IS_AppleClang
#define ANNOTATOR_ALIGNOF(X) __alignof__(X)
#elif ANNOTATOR_COMPILER_IS_MSVC
#define ANNOTATOR_ALIGNOF(X) __alignof(X)
#endif

#if ANNOTATOR_COMPILER_CXX_CONSTEXPR
#define ANNOTATOR_CONSTEXPR constexpr
#else
#define ANNOTATOR_CONSTEXPR
#endif

#if ANNOTATOR_COMPILER_CXX_FINAL
#define ANNOTATOR_FINAL final
#else
#define ANNOTATOR_FINAL
#endif

#if ANNOTATOR_COMPILER_CXX_NOEXCEPT
#define ANNOTATOR_NOEXCEPT noexcept
#define ANNOTATOR_NOEXCEPT_EXPR(X) noexcept(X)
#else
#define ANNOTATOR_NOEXCEPT
#define ANNOTATOR_NOEXCEPT_EXPR(X)
#endif

#if ANNOTATOR_COMPILER_CXX_NULLPTR
#define ANNOTATOR_NULLPTR nullptr
#else
#define ANNOTATOR_NULLPTR static_cast<void*>(0)
#endif

#if ANNOTATOR_COMPILER_CXX_THREAD_LOCAL
#define ANNOTATOR_THREAD_LOCAL thread_local
#elif ANNOTATOR_COMPILER_IS_GNU || ANNOTATOR_COMPILER_IS_Clang || \
    ANNOTATOR_COMPILER_IS_AppleClang
#define ANNOTATOR_THREAD_LOCAL __thread
#elif ANNOTATOR_COMPILER_IS_MSVC
#define ANNOTATOR_THREAD_LOCAL __declspec(thread)
#else
// ANNOTATOR_THREAD_LOCAL not defined for this configuration.
#endif

#endif

#endif
