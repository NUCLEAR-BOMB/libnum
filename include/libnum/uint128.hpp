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
	friend uint128 operator*(const uint128 left, const uint128 right) noexcept {
		using detail::mul;

        std::uint64_t high{};
        std::uint64_t low = mul(left.low, right.low, high);
        high += left.low * right.high;
        high += left.high * right.low;
        return uint128{high, low};
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
    friend uint128 operator<<(const uint128 left, const std::uint8_t cnt) noexcept {
        using detail::shl128, detail::shl;

        std::uint64_t x = shl128(left.low, left.high, cnt);
        std::uint64_t y = shl(left.low, cnt);

        std::uint64_t low = 0;
        if ((cnt & 64) == 0) { low = y; }
        std::uint64_t high = (cnt & 64) != 0 ? y : x;

        return uint128{high, low};
    }
    friend uint128 operator>>(uint128 left, const std::uint8_t cnt) noexcept {
        using detail::shr128, detail::shr;

        std::uint64_t x = shr128(left.low, left.high, cnt);
        std::uint64_t y = shr(left.high, cnt);

        std::uint64_t high = 0;
        if ((cnt & 64) == 0) { high = y; }
        std::uint64_t low = (cnt & 64) != 0 ? y : x;

        return uint128{high, low};
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
        // xor low2, low1
        // xor high2, high1
        // or low2, high2
        // setz al

		return ((left.low ^ right.low) | (left.high ^ right.high)) == 0;
	}
	friend bool operator!=(const uint128 left, const uint128 right) noexcept {
        // xor low2, low1
        // xor high2, high1
        // or low2, high2
        // setnz al

		return ((left.low ^ right.low) | (left.high ^ right.high)) != 0;
	}
	friend bool operator>(const uint128 left, const uint128 right) noexcept {
        // cmp low2, low1
        // sbb high2, high1
        // setc al
        using detail::subborrow, detail::bit_cast;

        std::uint8_t c{};
        (void)subborrow(right.high, left.high, std::uint8_t(right.low < left.low), c);
        return bit_cast<bool>(c);
	}
	friend bool operator>=(const uint128 left, const uint128 right) noexcept {
        // cmp low1, low2
        // sbb high1, high2
        // setnc al
        using detail::subborrow, detail::bit_cast;

        std::uint8_t c{};
        (void)subborrow(left.high, right.high, std::uint8_t(left.low < right.low), c);
        return !bit_cast<bool>(c); // msvc generates: setc al; test al,al; sete al instead of setnc
	}                      
    friend bool operator<(const uint128 left, const uint128 right) noexcept {
        // cmp low1, low2
        // sbb high1, high2
        // setc al
        using detail::subborrow, detail::bit_cast;

        std::uint8_t c{};
        (void)subborrow(left.high, right.high, std::uint8_t(left.low < right.low), c);
        return bit_cast<bool>(c);
    }
    friend bool operator<=(const uint128 left, const uint128 right) noexcept {
        // cmp low2, low1
        // sbb high2, high1
        // setnc al
        using detail::subborrow, detail::bit_cast;

        std::uint8_t c{};
        (void)subborrow(right.high, left.high, std::uint8_t(right.low < left.low), c);
        return !bit_cast<bool>(c); // msvc generates: setc al; test al,al; sete al instead of setnc
    }

};

}

