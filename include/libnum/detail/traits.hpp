#pragma once

#include <limits>

namespace libnum {

template<class T>
inline constexpr bool is_integral = std::numeric_limits<T>::is_specialized && std::numeric_limits<T>::is_integer;

template<class T>
inline constexpr bool is_signed = std::numeric_limits<T>::is_specialized && std::numeric_limits<T>::is_signed;

template<class T>
inline constexpr bool is_unsigned = std::numeric_limits<T>::is_specialized && !std::numeric_limits<T>::is_signed;

template<class T>
inline constexpr bool is_arithmetic = std::numeric_limits<T>::is_specialized;

template<class T>
inline constexpr bool is_floating_point = std::numeric_limits<T>::is_specialized && !std::numeric_limits<T>::is_integer;

}
