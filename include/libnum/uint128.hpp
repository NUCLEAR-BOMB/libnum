#pragma once

#include <libnum/detail/addcarry.hpp>

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

	friend uint128 operator+(uint128 left, const std::uint64_t right) noexcept {
		using detail::add_carry;

		std::uint8_t c{};
		left.first = add_carry(left.first, right, 0, c);
		left.second = add_carry(left.second, 0, c, c);
		return left;
	}
	friend uint128 operator+(const std::uint64_t left, uint128 right) noexcept {
		using detail::add_carry;

		std::uint8_t c{};
		right.first = add_carry(right.first, left, 0, c);
		right.second = add_carry(right.second, 0, c, c);
		return right;
	}

	friend bool operator==(const uint128 left, const uint128 right) noexcept {
		return (left.first == right.first) && (left.second == right.second);
	}
	friend bool operator!=(const uint128 left, const uint128 right) noexcept {
		return (left.first != right.first) || (left.second != right.second);
	}

};

}

