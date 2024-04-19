#pragma once

#include <libnum/detail/preprocessor.hpp>

#if LIBNUM_MSVC

#include <intrin.h>
#include <cstdint>

#pragma intrinsic(_umul128)

namespace libnum::detail {

LIBNUM_FORCEINLINE
inline std::uint64_t mul(std::uint64_t x, std::uint64_t y, std::uint64_t& high) noexcept {
    return ::_umul128(x, y, &high);
}

}

#endif
