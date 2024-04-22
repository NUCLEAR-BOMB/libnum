#pragma once

#include <libnum/detail/preprocessor.hpp>

#if LIBNUM_MSVC

#include <intrin.h>
#include <cstdint>

#pragma intrinsic(_udiv128)

namespace libnum::detail {

LIBNUM_FORCEINLINE
inline std::uint64_t div128(std::uint64_t hx, std::uint64_t lx, std::uint64_t y, std::uint64_t& rem) noexcept {
    return ::_udiv128(hx, lx, y, &rem);
}

}

#endif
