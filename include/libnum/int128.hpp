#pragma once

#include <libnum/detail/preprocessor.hpp>
#include <libnum/detail/intrinsics.hpp>

#include <cstdint>

namespace libnum {

class int128 {
private:
    std::uint64_t low_;
    std::int64_t high_;
public:

    constexpr int128(const std::int64_t x) noexcept
        : low_{std::uint64_t(x)}, high_{x < 0 ? -1 : 0} {}

    constexpr int128(const std::int64_t _high, const std::uint64_t _low) noexcept
        : low_{_low}, high_{_high} {}

    LIBNUM_FORCEINLINE constexpr std::uint64_t low() const noexcept { return low_; }
    LIBNUM_FORCEINLINE constexpr std::int64_t high() const noexcept { return high_; }

    friend int128 operator+(const int128 left, const int128 right) noexcept {
		using detail::addcarry;

		std::uint8_t c{};
		std::uint64_t low = addcarry(left.low_, right.low_, 0, c);
        std::uint64_t high = addcarry(std::uint64_t(left.high_), std::uint64_t(right.high_), c, c);
        return int128{std::int64_t(high), low};
	}

    friend bool operator==(const int128 left, const int128 right) noexcept {
        // xor low2, low1
        // xor high2, high1
        // or low2, high2
        // setz al
		return ((left.low_ ^ right.low_) | (left.high_ ^ right.high_)) == 0;
	}
	friend bool operator!=(const int128 left, const int128 right) noexcept {
        // xor low2, low1
        // xor high2, high1
        // or low2, high2
        // setnz al
		return ((left.low_ ^ right.low_) | (left.high_ ^ right.high_)) != 0;
	}


};

}
