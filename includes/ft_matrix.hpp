#pragma once

#include <print>
#include <vector>
#include <ranges>
#include <numeric>
#include <concepts>
#include <stdfloat>
#include <type_traits>
#include <gtest/gtest.h>

namespace concepts {
  
  template <typename T>
  concept Numeric = std::integral<T> || std::floating_point<T>;

  template <typename T>
  concept SignedNumeric = std::signed_integral<T> || std::floating_point<T>;

  template <Numeric T, Numeric U>
  using Promoted_Type = std::common_type_t<T, U>;

  template <SignedNumeric T, SignedNumeric U>
  using Promoted_Signed_Type = std::common_type_t<T, U>;

  template <typename R, typename T>
  concept flat_range = 
    Numeric<T> &&
    std::ranges::contiguous_range<R> &&
    std::convertible_to<std::ranges::range_value_t<R>, T>;

  template <typename R, typename T>
  concept nested_range =
    Numeric<T> &&
    std::ranges::forward_range<R> &&
    std::ranges::forward_range<std::ranges::range_value_t<R>> &&
    std::convertible_to<
      std::ranges::range_value_t<std::ranges::range_value_t<R>>, T>;
}

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Complex.hpp"

// Basic operations
template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] inline auto add(const Vector<T>& lhs, const Vector<U>& rhs) noexcept;

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] inline auto sub(const Vector<T>& lhs, const Vector<U>& rhs) noexcept;

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] inline auto scl(const Vector<T>& lhs, const U& scalar) noexcept;

// linear combinations
template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] inline concepts::Promoted_Type<T, U> linear_combination(T multiplicand, U scalar) noexcept;

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] Vector<concepts::Promoted_Type<T, U>> linear_combination(std::vector<Vector<T>>& candidates, std::vector<U>& scalars) noexcept;

// linear interpolations
template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] inline concepts::Promoted_Type<T, U> linear_interpolation(const T& u, const U& v, std::float32_t t) noexcept;

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] Vector<concepts::Promoted_Type<T, U>> linear_interpolation(const Vector<T>& u, const Vector<U>& v, std::float32_t t) noexcept;

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] Matrix<concepts::Promoted_Type<T, U>> linear_interpolation(const Matrix<T>& u, const Matrix<U>& v, std::float32_t t) noexcept;

// Dot products
template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto dot(const Vector<T>& lhs, const Vector<U>& rhs) -> concepts::Promoted_Type<T, U>;

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto dot(const Matrix<T>& lhs, const Matrix<U>& rhs) -> concepts::Promoted_Type<T, U>;

// Norms
template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto norm_1(const Vector<T>& lhs, const Vector<U>& rhs) -> concepts::Promoted_Type<T, U>;

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto norm(const Vector<T>& lhs, const Vector<U>& rhs) -> concepts::Promoted_Type<T, U>;

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto norm_inf(const Vector<T>& lhs, const Vector<U>& rhs) -> concepts::Promoted_Type<T, U>;

#include "basic_operations.tpp"
#include "linear_combinations.tpp"
#include "linear_interpolations.tpp"
#include "dot_products.tpp"

