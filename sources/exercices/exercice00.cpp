#include "ft_matrix.hpp"

auto vector_addition(
  const Vector<T>& lhs,
  const Vector<U>& rhs) {
  return lhs + rhs;
}

auto vector_substraction(
  const Vector<T>& lhs,
  const Vector<U>& rhs) {
  return lhs - rhs;
}

auto vector_scaling(
  const Vector<T>& lhs,
  const T& scalar) {
  return lhs * scalar;
}
