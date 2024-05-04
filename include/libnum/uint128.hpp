#pragma once

#include <libnum/detail/intrinsics.hpp>
#include <libnum/detail/traits.hpp>

#include <cstdint>
#include <limits>
#include <climits>

namespace libnum {

class alignas(16) uint128 {
#if !LIBNUM_HAS_BUILTIN_UINT128
private:
    std::uint64_t val[2];
public:
	constexpr uint128(const std::uint64_t first_) noexcept : val{0, first_} {}

	explicit constexpr uint128(const std::uint64_t _high, const std::uint64_t _low) noexcept
        : val{_high, _low} {}

    LIBNUM_FORCEINLINE constexpr std::uint64_t low() const noexcept { return val[1]; }
    LIBNUM_FORCEINLINE constexpr std::uint64_t high() const noexcept { return val[0]; }

	friend uint128 operator+(const uint128 left, const uint128 right) noexcept {
		using detail::addcarry;

		std::uint8_t c{};
		std::uint64_t low = addcarry(left.val[1], right.val[1], 0, c);
        std::uint64_t high = addcarry(left.val[0], right.val[0], c, c);
        return uint128{high, low};
	}
	friend uint128 operator-(const uint128 left, const uint128 right) noexcept {
		using detail::subborrow;

		std::uint8_t c{};
		std::uint64_t low = subborrow(left.val[1], right.val[1], 0, c);
        std::uint64_t high = subborrow(left.val[0], right.val[0], c, c);
        return uint128{high, low};
	}
	friend uint128 operator*(const uint128 left, const uint128 right) noexcept {
		using detail::mul;

        std::uint64_t high{};
        std::uint64_t low = mul(left.val[1], right.val[1], high);
        high += left.val[1] * right.val[0];
        high += left.val[0] * right.val[1];
        return uint128{high, low};
	}

    friend uint128 operator&(const uint128 left, const uint128 right) noexcept {
        std::uint64_t low = left.val[1] & right.val[1];
        std::uint64_t high = left.val[0] & right.val[0];
        return uint128{high, low};
    }
    friend uint128 operator|(const uint128 left, const uint128 right) noexcept {
        std::uint64_t low = left.val[1] | right.val[1];
        std::uint64_t high = left.val[0] | right.val[0];
        return uint128{high, low};
    }
    friend uint128 operator^(const uint128 left, const uint128 right) noexcept {
        std::uint64_t low = left.val[1] ^ right.val[1];
        std::uint64_t high = left.val[0] ^ right.val[0];
        return uint128{high, low};
    }
    friend uint128 operator<<(const uint128 left, const std::uint8_t cnt) noexcept {
        using detail::shl128, detail::shl;

        std::uint64_t x = shl128(left.val[1], left.val[0], cnt);
        std::uint64_t y = shl(left.val[1], cnt);

        std::uint64_t low = 0;
        if ((cnt & 64) == 0) { low = y; }
        std::uint64_t high = (cnt & 64) != 0 ? y : x;

        return uint128{high, low};
    }
    friend uint128 operator>>(const uint128 left, const std::uint8_t cnt) noexcept {
        using detail::shr128, detail::shr;

        std::uint64_t x = shr128(left.val[1], left.val[0], cnt);
        std::uint64_t y = shr(left.val[0], cnt);

        std::uint64_t high = 0;
        if ((cnt & 64) == 0) { high = y; }
        std::uint64_t low = (cnt & 64) != 0 ? y : x;

        return uint128{high, low};
    }
    friend uint128 operator~(const uint128 left) noexcept {
        return uint128{~left.val[0], ~left.val[1]};
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
		left.val[1] = addcarry(left.val[1], 1, 0, c);
        left.val[0] = addcarry(left.val[0], 0, c, c);
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
		left.val[1] = addcarry(left.val[1], std::uint64_t(-1), 0, c);
        left.val[0] = addcarry(left.val[0], std::uint64_t(-1), c, c);
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

		return ((left.val[1] ^ right.val[1]) | (left.val[0] ^ right.val[0])) == 0;
	}
	friend bool operator!=(const uint128 left, const uint128 right) noexcept {
        // xor low2, low1
        // xor high2, high1
        // or low2, high2
        // setnz al

		return ((left.val[1] ^ right.val[1]) | (left.val[0] ^ right.val[0])) != 0;
	}
	friend bool operator>(const uint128 left, const uint128 right) noexcept {
        // cmp low2, low1
        // sbb high2, high1
        // setc al
        using detail::subborrow, detail::bit_cast;

        std::uint8_t c{};
        (void)subborrow(right.val[0], left.val[0], std::uint8_t(right.val[1] < left.val[1]), c);
        return bit_cast<bool>(c);
	}
	friend bool operator>=(const uint128 left, const uint128 right) noexcept {
        // cmp low1, low2
        // sbb high1, high2
        // setnc al
        using detail::subborrow, detail::bit_cast;

        std::uint8_t c{};
        (void)subborrow(left.val[0], right.val[0], std::uint8_t(left.val[1] < right.val[1]), c);
        return !bit_cast<bool>(c); // msvc generates: setc al; test al,al; sete al instead of setnc
	}                      
    friend bool operator<(const uint128 left, const uint128 right) noexcept {
        // cmp low1, low2
        // sbb high1, high2
        // setc al
        using detail::subborrow, detail::bit_cast;

        std::uint8_t c{};
        (void)subborrow(left.val[0], right.val[0], std::uint8_t(left.val[1] < right.val[1]), c);
        return bit_cast<bool>(c);
    }
    friend bool operator<=(const uint128 left, const uint128 right) noexcept {
        // cmp low2, low1
        // sbb high2, high1
        // setnc al
        using detail::subborrow, detail::bit_cast;

        std::uint8_t c{};
        (void)subborrow(right.val[0], left.val[0], std::uint8_t(right.val[1] < left.val[1]), c);
        return !bit_cast<bool>(c); // msvc generates: setc al; test al,al; sete al instead of setnc
    }
#else
private:
    __uint128_t num;

    struct from_uint128 { explicit from_uint128() = default; };
    
    explicit constexpr uint128(const from_uint128, const __uint128_t num_) noexcept : num{num_} {}
public:
    constexpr uint128(const std::uint64_t first_) noexcept : num{first_} {}

	explicit constexpr uint128(const std::uint64_t _high, const std::uint64_t _low) noexcept
        : num{(__uint128_t{_high} << 64) | __uint128_t{_low}} {}

    LIBNUM_FORCEINLINE constexpr std::uint64_t low() const noexcept { return std::uint64_t(num); }
    LIBNUM_FORCEINLINE constexpr std::uint64_t high() const noexcept { return std::uint64_t(num >> 64U); }

	friend uint128 operator+(const uint128 left, const uint128 right) noexcept {
        return uint128{from_uint128{}, left.num + right.num};
	}
	friend uint128 operator-(const uint128 left, const uint128 right) noexcept {
        return uint128{from_uint128{}, left.num - right.num};
	}
	friend uint128 operator*(const uint128 left, const uint128 right) noexcept {
        return uint128{from_uint128{}, left.num * right.num};
	}

    friend uint128 operator&(const uint128 left, const uint128 right) noexcept {
        return uint128{from_uint128{}, left.num & right.num};
    }
    friend uint128 operator|(const uint128 left, const uint128 right) noexcept {
        return uint128{from_uint128{}, left.num | right.num};
    }
    friend uint128 operator^(const uint128 left, const uint128 right) noexcept {
        return uint128{from_uint128{}, left.num ^ right.num};
    }
    friend uint128 operator<<(const uint128 left, const std::uint8_t cnt) noexcept {
        return uint128{from_uint128{}, left.num << (cnt & 127U)};
    }
    friend uint128 operator>>(const uint128 left, const std::uint8_t cnt) noexcept {
        return uint128{from_uint128{}, left.num >> (cnt & 127U)};
    }
    friend uint128 operator~(const uint128 left) noexcept {
        return uint128{from_uint128{}, ~left.num};
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
        ++(left.num);
        return left;
    }
    friend uint128 operator++(uint128& left, int) noexcept {
        auto tmp = left;
        ++(left.num);
        return tmp;
    }
    friend uint128& operator--(uint128& left) noexcept {
        --(left.num);
        return left;
    }
    friend uint128 operator--(uint128& left, int) noexcept {
        auto tmp = left;
        --(left.num);
        return tmp;
    }

	friend bool operator==(const uint128 left, const uint128 right) noexcept {
        return left.num == right.num;
	}
	friend bool operator!=(const uint128 left, const uint128 right) noexcept {
        return left.num != right.num;
	}
	friend bool operator>(const uint128 left, const uint128 right) noexcept {
        return left.num > right.num;
	}
	friend bool operator>=(const uint128 left, const uint128 right) noexcept {
        return left.num >= right.num;
	}                      
    friend bool operator<(const uint128 left, const uint128 right) noexcept {
        return left.num < right.num;
    }
    friend bool operator<=(const uint128 left, const uint128 right) noexcept {
        return left.num <= right.num;
    }
#endif
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
