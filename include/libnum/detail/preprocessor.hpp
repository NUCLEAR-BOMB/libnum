#pragma once

#if defined(__clang__)
    #define LIBNUM_CLANG 1
    #define LIBNUM_GCC   0
    #define LIBNUM_MSVC  0
#elif defined(__GNUC__) || defined(__GNUG__)
    #define LIBNUM_CLANG 0
    #define LIBNUM_GCC   1
    #define LIBNUM_MSVC  0
#elif defined(_MSC_VER)
    #define LIBNUM_CLANG 0
    #define LIBNUM_GCC   0
    #define LIBNUM_MSVC  1
#else
    #define LIBNUM_CLANG 0
    #define LIBNUM_GCC   0
    #define LIBNUM_MSVC  0
#endif
