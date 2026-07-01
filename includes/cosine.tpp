// Dot product / product of magnitudes, aka euclidian norms
template <concepts::Numeric T, concepts::Numeric U>
std::float32_t angle_cos(const Vector<T>& lhs, const Vector<U>& rhs) {
  return dot(lhs, rhs) / (lhs.norm() * rhs.norm()); 
}

// Using Froebenius norm form matrices
template <concepts::Numeric T, concepts::Numeric U>
std::float32_t angle_cos(const Matrix<T>& lhs, const Matrix<U>& rhs) {

  assert(lhs.get_shape() == rhs.get_shape() && "Can't compute dot product for matrices with different shapes");

  return dot(lhs, rhs) / (lhs.norm() * rhs.norm()); 
}
