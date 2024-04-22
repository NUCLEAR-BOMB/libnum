#pragma once

#include <libnum/detail/preprocessor.hpp>
#include <intrin.h>
#include <cstdint>

#pragma intrinsic(_addcarry_u64, _subborrow_u64, _umul128, _udiv128)

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
inline std::uint64_t rotl(std::uint64_t x, std::uint64_t count) noexcept {
    return ::_rotl64(x, count);
}
LIBNUM_FORCEINLINE
inline std::uint32_t rotl(std::uint32_t x, std::uint32_t count) noexcept {
    return ::_rotl(x, count);
}
LIBNUM_FORCEINLINE
inline std::uint16_t rotl(std::uint16_t x, std::uint16_t count) noexcept {
    return ::_rotl16(x, count);
}
LIBNUM_FORCEINLINE
inline std::uint8_t rotl(std::uint8_t x, std::uint8_t count) noexcept {
    return ::_rotl8(x, count);
}

LIBNUM_FORCEINLINE
inline std::uint64_t rotr(std::uint64_t x, std::uint64_t count) noexcept {
    return ::_rotr64(x, count);
}
LIBNUM_FORCEINLINE
inline std::uint32_t rotr(std::uint32_t x, std::uint32_t count) noexcept {
    return ::_rotr(x, count);
}
LIBNUM_FORCEINLINE
inline std::uint16_t rotr(std::uint16_t x, std::uint16_t count) noexcept {
    return ::_rotr16(x, count);
}
LIBNUM_FORCEINLINE
inline std::uint8_t rotr(std::uint8_t x, std::uint8_t count) noexcept {
    return ::_rotr8(x, count);
}

}
