#pragma once

#include <libnum/detail/addcarry.hpp>
#include <libnum/detail/subborrow.hpp>
#include <libnum/detail/mul.hpp>

#include <cstdint>

namespace libnum {

class uint128 {
public:
	std::uint64_t low;
	std::uint64_t high;

	constexpr uint128(const std::uint64_t first_) noexcept : low{first_}, high{} {}

	explicit constexpr uint128(const std::uint64_t high_, const std::uint64_t low_) noexcept
		: low{low_}, high{high_} {}

	LIBNUM_FORCEINLINE
	friend uint128 operator+(uint128 left, const uint128 right) noexcept {
		using detail::addcarry;

		std::uint8_t c{};
		left.low = addcarry(left.low, right.low, 0, c);
        left.high = addcarry(left.high, right.high, c, c);
		return left;
	}
    LIBNUM_FORCEINLINE
	friend uint128 operator-(uint128 left, const uint128 right) noexcept {
		using detail::subborrow;

		std::uint8_t c{};
		left.low = subborrow(left.low, right.low, 0, c);
        left.high = subborrow(left.high, right.high, c, c);
		return left;
	}
    LIBNUM_FORCEINLINE
	friend uint128 operator*(uint128 left, const uint128 right) noexcept {
		using detail::mul;

        std::uint64_t high{};
        left.low = mul(left.low, right.low, high);
        left.high = (left.high * right.high) + high;
        return left;
	}

	friend bool operator==(const uint128 left, const uint128 right) noexcept {
		return (left.low == right.low) && (left.high == right.high);
	}
	friend bool operator!=(const uint128 left, const uint128 right) noexcept {
		return (left.low != right.low) || (left.high != right.high);
	}

};

}

