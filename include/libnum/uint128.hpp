#pragma once

#include <libnum/detail/intrinsics.hpp>

#include <cstdint>
#include <limits>
#include <climits>

namespace libnum {

class uint128 {
public:
	std::uint64_t low;
	std::uint64_t high;

	constexpr uint128(const std::uint64_t first_) noexcept : low{first_}, high{} {}

	explicit constexpr uint128(const std::uint64_t high_, const std::uint64_t low_) noexcept
		: low{low_}, high{high_} {}

	friend uint128 operator+(const uint128 left, const uint128 right) noexcept {
		using detail::addcarry;

		std::uint8_t c{};
		std::uint64_t low = addcarry(left.low, right.low, 0, c);
        std::uint64_t high = addcarry(left.high, right.high, c, c);
        return uint128{high, low};
	}
	friend uint128 operator-(const uint128 left, const uint128 right) noexcept {
		using detail::subborrow;

		std::uint8_t c{};
		std::uint64_t low = subborrow(left.low, right.low, 0, c);
        std::uint64_t high = subborrow(left.high, right.high, c, c);
        return uint128{high, low};
	}
	friend uint128 operator*(const uint128 left, const uint128 right) noexcept {
		using detail::mul;

        std::uint64_t high{};
        std::uint64_t low = mul(left.low, right.low, high);
        high += left.low * right.high;
        high += left.high * right.low;
        return uint128{high, low};
	}
    friend uint128 operator/(const uint128 left, const std::uint64_t right) noexcept {
        return uint128{left.high / right, left.low / right};
    }

    friend uint128 operator&(const uint128 left, const uint128 right) noexcept {
        std::uint64_t low = left.low & right.low;
        std::uint64_t high = left.high & right.high;
        return uint128{high, low};
    }
    friend uint128 operator|(const uint128 left, const uint128 right) noexcept {
        std::uint64_t low = left.low | right.low;
        std::uint64_t high = left.high | right.high;
        return uint128{high, low};
    }
    friend uint128 operator^(const uint128 left, const uint128 right) noexcept {
        std::uint64_t low = left.low ^ right.low;
        std::uint64_t high = left.high ^ right.high;
        return uint128{high, low};
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
    friend uint128 operator>>(const uint128 left, const std::uint8_t cnt) noexcept {
        using detail::shr128, detail::shr;

        std::uint64_t x = shr128(left.low, left.high, cnt);
        std::uint64_t y = shr(left.high, cnt);

        std::uint64_t high = 0;
        if ((cnt & 64) == 0) { high = y; }
        std::uint64_t low = (cnt & 64) != 0 ? y : x;

        return uint128{high, low};
    }
    friend uint128 operator~(const uint128 left) noexcept {
        return uint128{~left.high, ~left.low};
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

    friend uint128& operator++(uint128& left) noexcept {
        using detail::addcarry;

		std::uint8_t c{};
		left.low = addcarry(left.low, 1, 0, c);
        left.high = addcarry(left.high, 0, c, c);
        return left;
    }
    friend uint128 operator++(uint128& left, int) noexcept {
        auto tmp = left;
        ++left;
        return tmp;
    }
    friend uint128& operator--(uint128& left) noexcept {
        using detail::addcarry;

		std::uint8_t c{};
		left.low = addcarry(left.low, std::uint64_t(-1), 0, c);
        left.high = addcarry(left.high, std::uint64_t(-1), c, c);
        return left;
    }
    friend uint128 operator--(uint128& left, int) noexcept {
        auto tmp = left;
        --left;
        return tmp;
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

template<>
class ::std::numeric_limits<libnum::uint128> {
    using uint64_limits = ::std::numeric_limits<std::uint64_t>;
public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr std::float_denorm_style has_denorm = std::denorm_absent;
    static constexpr bool has_denorm_loss = false;
    static constexpr std::float_round_style round_style = std::round_toward_zero;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = true;
    static constexpr int digits = CHAR_BIT * 16;
    static constexpr int digits10 = 38;
    static constexpr int max_digits10 = 0;
    static constexpr int radix = 2;
    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;
    static constexpr bool traps = uint64_limits::traps;
    static constexpr bool tinyness_before = false;

    static constexpr libnum::uint128(min)() noexcept {
        return libnum::uint128{0, 0};
    }
    static constexpr libnum::uint128 lowest() noexcept {
        return libnum::uint128{0, 0};
    }
    static constexpr libnum::uint128(max)() noexcept {
        return libnum::uint128{uint64_limits::max(), uint64_limits::max()};
    }
    static constexpr libnum::uint128 epsilon() noexcept {
        return libnum::uint128{0, 0};
    }
    static constexpr libnum::uint128 round_error() noexcept {
        return libnum::uint128{0, 0};
    }
    static constexpr libnum::uint128 infinity() noexcept {
        return libnum::uint128{0, 0};
    }
    static constexpr libnum::uint128 quiet_NaN() noexcept {
        return libnum::uint128{0, 0};
    }
    static constexpr libnum::uint128 signaling_NaN() noexcept {
        return libnum::uint128{0, 0};
    }
    static constexpr libnum::uint128 denorm_min() noexcept {
        return libnum::uint128{0, 0};
    }
};
