#pragma once

#if defined(__clang__)
    #define LIBNUM_CLANG 1
    #define LIBNUM_GCC   0
    #define LIBNUM_MSVC  0

    #define LIBNUM_FORCEINLINE [[clang::always_inline]]
    #define LIBNUM_HAS_BUILTIN_UINT128 1
#elif defined(__GNUC__) || defined(__GNUG__)
    #define LIBNUM_CLANG 0
    #define LIBNUM_GCC   1
    #define LIBNUM_MSVC  0

    #define LIBNUM_FORCEINLINE [[gnu::always_inline]]
    #define LIBNUM_HAS_BUILTIN_UINT128 1
#elif defined(_MSC_VER)
    #define LIBNUM_CLANG 0
    #define LIBNUM_GCC   0
    #define LIBNUM_MSVC  1

    #define LIBNUM_FORCEINLINE [[msvc::forceinline]]
    #define LIBNUM_HAS_BUILTIN_UINT128 0
#else
    #define LIBNUM_CLANG 0
    #define LIBNUM_GCC   0
    #define LIBNUM_MSVC  0
    
    #define LIBNUM_FORCEINLINE
    #define LIBNUM_HAS_BUILTIN_UINT128 0
#endif
