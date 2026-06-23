#pragma once

#include <print>
#include <concepts>
#include <type_traits>
#include <gtest/gtest.h>

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Complex.hpp"

template <vector::Numeric T, vector::Numeric U>
auto vector_addition(const Vector<T>& lhs, const Vector<U>& rhs) {
	return lhs + rhs;
}

template <vector::Numeric T, vector::Numeric U>
auto vector_substraction(const Vector<T>& lhs, const Vector<U>& rhs) {
	return lhs - rhs;
}

template <vector::Numeric T, vector::Numeric U>
auto vector_scaling(const Vector<T>& lhs, const U& scalar) {
	return lhs * scalar;
}

// Using R pour s'assurer du type promotion ?
template <vector::Numeric T>
Vector<T> linear_combination(std::vector<Vector<T>>& candidates, std::vector<T>& scalars);
