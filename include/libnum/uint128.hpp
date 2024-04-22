#pragma once

#include <libnum/detail/intrinsics.hpp>

#include <cstdint>

namespace libnum {

class uint128 {
public:
	std::uint64_t low;
	std::uint64_t high;

	constexpr uint128(const std::uint64_t first_) noexcept : low{first_}, high{} {}

	explicit constexpr uint128(const std::uint64_t high_, const std::uint64_t low_) noexcept
		: low{low_}, high{high_} {}

	friend uint128 operator+(uint128 left, const uint128 right) noexcept {
		using detail::addcarry;

		std::uint8_t c{};
		left.low = addcarry(left.low, right.low, 0, c);
        left.high = addcarry(left.high, right.high, c, c);
		return left;
	}
	friend uint128 operator-(uint128 left, const uint128 right) noexcept {
		using detail::subborrow;

		std::uint8_t c{};
		left.low = subborrow(left.low, right.low, 0, c);
        left.high = subborrow(left.high, right.high, c, c);
		return left;
	}
	friend uint128 operator*(uint128 left, const uint128 right) noexcept {
		using detail::mul;

        std::uint64_t high{};
        left.low = mul(left.low, right.low, high);
        left.high = (left.high * right.high) + high;
        return left;
	}
    // LIBNUM_FORCEINLINE
	// friend uint128 operator/(uint128 left, uint128 right) noexcept {
	// 	using detail::div128;
    // 
    //     [[maybe_unused]] std::uint64_t rem{};
    //     if (left.high < right.low) {
    //         left.low = div128(left.high, left.low, right.low, rem);
    //     } else {
    //         left.low = left.low / right.low;
    //     }
    //     left.high = right.high != 0 ? (left.high / right.high) : 0;
    //     return left;
	// }

    friend uint128 operator&(uint128 left, const uint128 right) noexcept {
        left.low &= right.low;
        left.high &= right.high;
        return left;
    }
    friend uint128 operator|(uint128 left, const uint128 right) noexcept {
        left.low |= right.low;
        left.high |= right.high;
        return left;
    }
    friend uint128 operator^(uint128 left, const uint128 right) noexcept {
        left.low ^= right.low;
        left.high ^= right.high;
        return left;
    }
    friend uint128 operator<<(uint128 left, const std::uint8_t cnt) noexcept {
        using detail::shl128;

        std::uint64_t x = shl128(left.low, left.high, (cnt % 64U));
        std::uint64_t y = left.low << (cnt % 64U);

        left.low = (cnt & 64) == 0 ? y : 0;
        left.high = (cnt & 64) != 0 ? y : x;

        return left;
    }
    friend uint128 operator>>(uint128 left, const std::uint8_t cnt) noexcept {
        using detail::shr128;

        std::uint64_t x = shr128(left.low, left.high, (cnt % 64U));
        std::uint64_t y = left.high >> (cnt % 64U);

        left.high = (cnt & 64) == 0 ? y : 0;
        left.low = (cnt & 64) != 0 ? y : x;

        return left;
    }
    friend uint128 operator~(uint128 left) noexcept {
        left.low = ~left.low;
        left.high = ~left.high;
        return left;
    }

    friend uint128& operator&=(uint128& left, const uint128 right) noexcept {
        return (left = left & right);
    }
    friend uint128& operator|=(uint128& left, const uint128 right) noexcept {
        return (left = left | right);
    }
    friend uint128& operator^=(uint128& left, const uint128 right) noexcept {
        return (left = left ^ right);
    }
    friend uint128& operator<<=(uint128& left, const std::uint8_t right) noexcept {
        return (left = left << right);
    }
    friend uint128& operator>>=(uint128& left, const std::uint8_t right) noexcept {
        return (left = left >> right);
    }

	friend bool operator==(const uint128 left, const uint128 right) noexcept {
		return (left.low == right.low) && (left.high == right.high);
	}
	friend bool operator!=(const uint128 left, const uint128 right) noexcept {
		return (left.low != right.low) || (left.high != right.high);
	}
    [[msvc::noinline]]
	friend bool operator>(const uint128 left, const uint128 right) noexcept {
        // __debugbreak();
        using detail::subborrow;

        std::uint8_t c{};
        (void)subborrow(right.high, left.high, left.low > right.low, c);
        return static_cast<bool>(c);
	}

};

}

