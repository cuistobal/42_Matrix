#pragma once

#include <print>
#include <concepts>
#include <stdfloat>
#include <type_traits>
#include <gtest/gtest.h>

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Complex.hpp"

namespace concepts {
  
  template <typename T>
  concept Numeric = std::integral<T> || std::floating_point<T>;

  template <Numeric T, Numeric U>
  using Promoted_Type = std::common_type_t<T, U>;
}

// Basic operations
template <concepts::Numeric T, concepts::Numeric U>
auto vector_addition(const Vector<T>& lhs, const Vector<U>& rhs);

template <concepts::Numeric T, concepts::Numeric U>
auto vector_substraction(const Vector<T>& lhs, const Vector<U>& rhs);

template <concepts::Numeric T, concepts::Numeric U>
auto vector_scaling(const Vector<T>& lhs, const U& scalar);


// linear combinations

template <concepts::Numeric T, concepts::Numeric U>
inline concepts::Promoted_Type<T, U> linear_combination(T multiplicand, U scalar);

template <concepts::Numeric T, concepts::Numeric U>
vector::PVector<T, U> linear_combination(std::vector<Vector<T>>& candidates, std::vector<U>& scalars);

// linear interpolations
template <concepts::Numeric T, concepts::Numeric U>
inline concepts::Promoted_Type<T, U> linear_interpolation(const T& u, const U& v, std::float32_t t);

template <concepts::Numeric T, concepts::Numeric U>
vector::PVector<T, U> linear_interpolation(const Vector<T>& u, const Vector<U>& v, std::float32_t t);

template <concepts::Numeric T, concepts::Numeric U>
matrix::PMatrix<T, U> linear_interpolation(const Matrix<T>& u, const Matrix<U>& v, std::float32_t t);

#include "basic_operations.tpp"
#include "linear_combinations.tpp"
#include "linear_interpolations.tpp"
