#pragma once

#include <libnum/detail/preprocessor.hpp>
#include <cstdint>
#include <cstring>

#if LIBNUM_MSVC
    #include <intrin.h>

    #pragma intrinsic( \
        _addcarry_u64, _subborrow_u64, _umul128, _udiv128, \
        _rotl64, _rotl, _rotl16, _rotl8, _rotr64, _rotr, _rotr16, _rotr8, \
        __shiftleft128, __shiftright128, \
        _BitScanForward64, \
        memcpy \
    )
#endif

namespace libnum::detail {

LIBNUM_FORCEINLINE
inline std::uint64_t addcarry(std::uint64_t x, std::uint64_t y, unsigned char carry_in, unsigned char& carry_out) noexcept {
#if LIBNUM_MSVC
    std::uint64_t out;
	carry_out = ::_addcarry_u64(carry_in, x, y, &out);
	return out;
#else
    unsigned long long cf{};
    std::uint64_t out = __builtin_addcll(x, y, carry_in, &cf);
    carry_out = std::uint8_t(cf);
    return out;
#endif
}

LIBNUM_FORCEINLINE
inline std::uint64_t subborrow(std::uint64_t x, std::uint64_t y, unsigned char carry_in, unsigned char& carry_out) noexcept {
#if LIBNUM_MSVC
    std::uint64_t out;
	carry_out = ::_subborrow_u64(carry_in, x, y, &out);
	return out;
#else
    unsigned long long cf{};
    std::uint64_t out = __builtin_subcll(x, y, carry_in, &cf);
    carry_out = std::uint8_t(cf);
    return out;
#endif
}

LIBNUM_FORCEINLINE
inline std::uint64_t mul(std::uint64_t x, std::uint64_t y, std::uint64_t& high) noexcept {
#if LIBNUM_MSVC
    return ::_umul128(x, y, &high);
#else
    __uint128_t result = __uint128_t{x} * __uint128_t{y};
    high = result >> 64U;
    return std::uint64_t(result);
#endif
}

LIBNUM_FORCEINLINE
inline std::uint64_t rotl(std::uint64_t x, std::uint8_t count) noexcept {
#if LIBNUM_MSVC
    return ::_rotl64(x, count);
#else
    return __builtin_rotateleft64(x, count);
#endif
}
LIBNUM_FORCEINLINE
inline std::uint32_t rotl(std::uint32_t x, std::uint8_t count) noexcept {
#if LIBNUM_MSVC
    return ::_rotl(x, count);
#else
    return __builtin_rotateleft32(x, count);
#endif
}
LIBNUM_FORCEINLINE
inline std::uint16_t rotl(std::uint16_t x, std::uint8_t count) noexcept {
#if LIBNUM_MSVC
    return ::_rotl16(x, count);
#else
    return __builtin_rotateleft16(x, count);
#endif
}
LIBNUM_FORCEINLINE
inline std::uint8_t rotl(std::uint8_t x, std::uint8_t count) noexcept {
#if LIBNUM_MSVC
    return ::_rotl8(x, count);
#else
    return __builtin_rotateleft8(x, count);
#endif
}

LIBNUM_FORCEINLINE
inline std::uint64_t rotr(std::uint64_t x, std::uint8_t count) noexcept {
#if LIBNUM_MSVC
    return ::_rotr64(x, count);
#else
    return __builtin_rotateright64(x, count);
#endif
}
LIBNUM_FORCEINLINE
inline std::uint32_t rotr(std::uint32_t x, std::uint8_t count) noexcept {
#if LIBNUM_MSVC
    return ::_rotr(x, count);
#else
    return __builtin_rotateright32(x, count);
#endif
}
LIBNUM_FORCEINLINE
inline std::uint16_t rotr(std::uint16_t x, std::uint8_t count) noexcept {
#if LIBNUM_MSVC
    return ::_rotr16(x, count);
#else
    return __builtin_rotateright16(x, count);
#endif
}
LIBNUM_FORCEINLINE
inline std::uint8_t rotr(std::uint8_t x, std::uint8_t count) noexcept {
#if LIBNUM_MSVC
    return ::_rotr8(x, count);
#else
    return __builtin_rotateright8(x, count);
#endif
}

LIBNUM_FORCEINLINE
inline std::uint64_t shl128(std::uint64_t lx, std::uint64_t hx, std::uint8_t count) noexcept {
#if LIBNUM_MSVC
    return ::__shiftleft128(lx, hx, count);
#else
    return std::uint64_t(((__uint128_t{lx} | (__uint128_t{hx} << 64U)) << (count & 63U)) >> 64U);
#endif
}
LIBNUM_FORCEINLINE
inline std::uint64_t shr128(std::uint64_t lx, std::uint64_t hx, std::uint8_t count) noexcept {
#if LIBNUM_MSVC
    return ::__shiftright128(lx, hx, count);
#else
    return std::uint64_t((__uint128_t{lx} | (__uint128_t{hx} << 64U)) >> (count & 63U));
#endif
}

LIBNUM_FORCEINLINE
inline std::uint64_t shl(std::uint64_t x, std::uint8_t cnt) noexcept {
#if LIBNUM_MSVC
    return ::__ll_lshift(x, cnt);
#else
    return x << (cnt & 63U);
#endif
}
LIBNUM_FORCEINLINE
inline std::uint64_t shr(std::uint64_t x, std::uint8_t cnt) noexcept {
#if LIBNUM_MSVC
    return ::__ull_rshift(x, cnt);
#else
    return x >> (cnt & 63U);
#endif
}
LIBNUM_FORCEINLINE
inline std::int64_t sar(std::int64_t x, std::uint8_t cnt) noexcept {
#if LIBNUM_MSVC
    return ::__ll_rshift(x, cnt);
#else
    return x >> (cnt & 63U);
#endif
}

template<class To, class From> LIBNUM_FORCEINLINE
To bit_cast(const From& from) noexcept {
    static_assert(sizeof(To) == sizeof(From));

    To res;
    ::memcpy(&res, &from, sizeof(To));
    return res;
}

}
