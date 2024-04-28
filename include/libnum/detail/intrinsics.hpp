#pragma once

#include <libnum/detail/preprocessor.hpp>
#include <intrin.h>
#include <cstdint>
#include <cstring>

#pragma intrinsic( \
    _addcarry_u64, _subborrow_u64, _umul128, _udiv128, \
    _rotl64, _rotl, _rotl16, _rotl8, _rotr64, _rotr, _rotr16, _rotr8, \
    __shiftleft128, __shiftright128, \
    _BitScanForward64, \
    memcpy \
)

namespace libnum::detail {

LIBNUM_FORCEINLINE
inline std::uint64_t addcarry(std::uint64_t x, std::uint64_t y, unsigned char carry_in, unsigned char& carry_out) noexcept {
	std::uint64_t out;
	carry_out = ::_addcarry_u64(carry_in, x, y, &out);
	return out;
}

LIBNUM_FORCEINLINE
inline std::uint64_t subborrow(std::uint64_t x, std::uint64_t y, unsigned char carry_in, unsigned char& carry_out) noexcept {
	std::uint64_t out;
	carry_out = ::_subborrow_u64(carry_in, x, y, &out);
	return out;
}

LIBNUM_FORCEINLINE
inline std::uint64_t mul(std::uint64_t x, std::uint64_t y, std::uint64_t& high) noexcept {
    return ::_umul128(x, y, &high);
}

LIBNUM_FORCEINLINE
inline std::uint64_t div128(std::uint64_t hx, std::uint64_t lx, std::uint64_t y, std::uint64_t& rem) noexcept {
    return ::_udiv128(hx, lx, y, &rem);
}

LIBNUM_FORCEINLINE
inline std::uint64_t rotl(std::uint64_t x, std::uint8_t count) noexcept {
    return ::_rotl64(x, count);
}
LIBNUM_FORCEINLINE
inline std::uint32_t rotl(std::uint32_t x, std::uint8_t count) noexcept {
    return ::_rotl(x, count);
}
LIBNUM_FORCEINLINE
inline std::uint16_t rotl(std::uint16_t x, std::uint8_t count) noexcept {
    return ::_rotl16(x, count);
}
LIBNUM_FORCEINLINE
inline std::uint8_t rotl(std::uint8_t x, std::uint8_t count) noexcept {
    return ::_rotl8(x, count);
}

LIBNUM_FORCEINLINE
inline std::uint64_t rotr(std::uint64_t x, std::uint8_t count) noexcept {
    return ::_rotr64(x, count);
}
LIBNUM_FORCEINLINE
inline std::uint32_t rotr(std::uint32_t x, std::uint8_t count) noexcept {
    return ::_rotr(x, count);
}
LIBNUM_FORCEINLINE
inline std::uint16_t rotr(std::uint16_t x, std::uint8_t count) noexcept {
    return ::_rotr16(x, count);
}
LIBNUM_FORCEINLINE
inline std::uint8_t rotr(std::uint8_t x, std::uint8_t count) noexcept {
    return ::_rotr8(x, count);
}

LIBNUM_FORCEINLINE
inline std::uint64_t shl128(std::uint64_t lx, std::uint64_t hx, std::uint8_t count) noexcept {
    return ::__shiftleft128(lx, hx, count);
}
LIBNUM_FORCEINLINE
inline std::uint64_t shr128(std::uint64_t lx, std::uint64_t hx, std::uint8_t count) noexcept {
    return ::__shiftright128(lx, hx, count);
}

LIBNUM_FORCEINLINE
inline std::uint64_t shl(std::uint64_t x, std::uint8_t cnt) noexcept {
    return ::__ll_lshift(x, cnt);
}
LIBNUM_FORCEINLINE
inline std::uint64_t shr(std::uint64_t x, std::uint8_t cnt) noexcept {
    return ::__ull_rshift(x, cnt);
}
LIBNUM_FORCEINLINE
inline std::int64_t sar(std::int64_t x, std::uint8_t cnt) noexcept {
    return ::__ll_rshift(x, cnt);
}

template<class To, class From> LIBNUM_FORCEINLINE
To bit_cast(const From& from) noexcept {
    static_assert(sizeof(To) == sizeof(From));

    To res;
    ::memcpy(&res, &from, sizeof(To));
    return res;
}

LIBNUM_FORCEINLINE
inline std::uint8_t bsf(std::uint64_t x) noexcept {
    unsigned long out{};
    (void)::_BitScanForward64(&out, x);
    return static_cast<std::uint8_t>(out);
}
LIBNUM_FORCEINLINE
inline std::uint8_t bsf(std::uint32_t x) noexcept {
    unsigned long out{};
    (void)::_BitScanForward(&out, x);
    return static_cast<std::uint8_t>(out);
}

}
