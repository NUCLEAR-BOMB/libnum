#pragma once

#include <libnum/detail/preprocessor.hpp>

#if LIBNUM_MSVC

#include <intrin.h>
#include <cstdint>

#pragma intrinsic(_subborrow_u64)

namespace libnum::detail {

LIBNUM_FORCEINLINE
inline std::uint64_t subborrow(std::uint64_t x, std::uint64_t y, unsigned char carry_in, unsigned char& carry_out) noexcept {
	std::uint64_t out;
	carry_out = ::_subborrow_u64(carry_in, x, y, &out);
	return out;
}

}

#endif
