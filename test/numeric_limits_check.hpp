#pragma once

#include <doctest/doctest.h>

#include <limits>
#include <type_traits>

namespace {

template<class T>
constexpr void numeric_limits_check() noexcept {
    using limits = std::numeric_limits<T>;
    CHECK((std::is_same_v<decltype(limits::is_specialized), const bool> && limits::is_specialized) == true);
    CHECK(std::is_same_v<decltype(limits::is_signed), const bool>);
    CHECK(std::is_same_v<decltype(limits::is_integer), const bool>);
    CHECK(std::is_same_v<decltype(limits::is_exact), const bool>);
    CHECK(std::is_same_v<decltype(limits::has_infinity), const bool>);
    CHECK(std::is_same_v<decltype(limits::has_quiet_NaN), const bool>);
    CHECK(std::is_same_v<decltype(limits::has_signaling_NaN), const bool>);
    CHECK(std::is_same_v<decltype(limits::has_denorm), const std::float_denorm_style>);
    CHECK(std::is_same_v<decltype(limits::has_denorm_loss), const bool>);
    CHECK(std::is_same_v<decltype(limits::round_style), const std::float_round_style>);
    CHECK(std::is_same_v<decltype(limits::is_iec559), const bool>);
    CHECK(std::is_same_v<decltype(limits::is_bounded), const bool>);
    CHECK(std::is_same_v<decltype(limits::is_modulo), const bool>);
    CHECK(std::is_same_v<decltype(limits::digits), const int>);
    CHECK(std::is_same_v<decltype(limits::digits10), const int>);
    CHECK(std::is_same_v<decltype(limits::max_digits10), const int>);
    CHECK(std::is_same_v<decltype(limits::radix), const int>);
    CHECK(std::is_same_v<decltype(limits::min_exponent), const int>);
    CHECK(std::is_same_v<decltype(limits::min_exponent10), const int>);
    CHECK(std::is_same_v<decltype(limits::max_exponent), const int>);
    CHECK(std::is_same_v<decltype(limits::max_exponent10), const int>);
    CHECK(std::is_same_v<decltype(limits::traps), const bool>);
    CHECK(std::is_same_v<decltype(limits::tinyness_before), const bool>);

    CHECK(std::is_same_v<decltype(limits::min()), T>);
    CHECK(std::is_same_v<decltype(limits::lowest()), T>);
    CHECK(std::is_same_v<decltype(limits::max()), T>);
    CHECK(std::is_same_v<decltype(limits::epsilon()), T>);
    CHECK(std::is_same_v<decltype(limits::round_error()), T>);
    CHECK(std::is_same_v<decltype(limits::infinity()), T>);
    CHECK(std::is_same_v<decltype(limits::quiet_NaN()), T>);
    CHECK(std::is_same_v<decltype(limits::signaling_NaN()), T>);
    CHECK(std::is_same_v<decltype(limits::denorm_min()), T>);
}

}
