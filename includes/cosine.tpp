// Dot product / product of magnitudes, aka euclidian norms
template <concepts::Numeric T, concepts::Numeric U>
std::float32_t angle_cos(Vector<T>& lhs, Vector<U>& rhs) {
  return dot(lhs, rhs) / (lhs.norm() * rhs.norm()); 
}
