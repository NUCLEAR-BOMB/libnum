#pragma once

#include <libnum/detail/addcarry.hpp>
#include <libnum/detail/subborrow.hpp>

#include <cstdint>

namespace libnum {

class uint128 {
private:
	std::uint64_t first;
	std::uint64_t second;
public:
	constexpr uint128(const std::uint64_t first_) noexcept : first{first_}, second{} {}

	explicit constexpr uint128(const std::uint64_t first_, const std::uint64_t second_) noexcept
		: first{first_}, second{second_} {}

	LIBNUM_FORCEINLINE
	friend uint128 operator+(uint128 left, const uint128 right) noexcept {
		using detail::addcarry;

		std::uint8_t c{};
		left.first = addcarry(left.first, right.first, 0, c);
        left.second = addcarry(left.second, right.second, c, c);
		return left;
	}
    LIBNUM_FORCEINLINE
	friend uint128 operator-(uint128 left, const uint128 right) noexcept {
		using detail::subborrow;

		std::uint8_t c{};
		left.first = subborrow(left.first, right.first, 0, c);
        left.second = subborrow(left.second, right.second, c, c);
		return left;
	}

	friend bool operator==(const uint128 left, const uint128 right) noexcept {
		return (left.first == right.first) && (left.second == right.second);
	}
	friend bool operator!=(const uint128 left, const uint128 right) noexcept {
		return (left.first != right.first) || (left.second != right.second);
	}

};

}

