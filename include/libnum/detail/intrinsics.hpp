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

}
