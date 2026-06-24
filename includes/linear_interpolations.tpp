template <concepts::Numeric T, concepts::Numeric U>
inline concepts::Promoted_Type<T, U> linear_interpolation(const T& u, const U& v, std::float32_t t) {
  using R = concepts::Promoted_Type<T, U>;
  return 
    static_cast<R>(
      linear_combination(u, t - 1) + 
      linear_combination(v, t)); 
}

template <concepts::Numeric T, concepts::Numeric U>
vector::PVector<T, U> linear_interpolation(const Vector<T>& u, const Vector<U>& v, std::float32_t t) { 
  
  if (u.get_dimensions() != v.get_dimensions()) {
    throw
      std::invalid_argument(
        "Can't interpolate matrixes with different shapes");
  }

  using R = vector::promoted_type<T, U>;

  auto interpolation = 
    std::views::zip(u.get_data(), v.get_data()) |
    std::views::transform([t](auto&& tuple) {
      auto&& [lhs, rhs] = tuple; 
      return linear_combination(lhs, 1.0f - t) + linear_combination(rhs, t);
    }
  );

  return Vector<R>{std::ranges::to<std::vector<R> >(interpolation)};
}

template <concepts::Numeric T, concepts::Numeric U>
matrix::PMatrix<T, U> linear_interpolation(const Matrix<T>& u, const Matrix<U>& v, std::float32_t t) { 

  if (u.get_shape() != v.get_shape()) {
    throw
      std::invalid_argument(
        "Can't interpolate matrixes with different shapes");
  } 

  using R = matrix::promoted_type<T, U>;

  auto interpolation = 
    std::views::zip(u.get_data(), v.get_data) |
    std::views::transform([t](auto&& tuple) {
      auto&& [lhs, rhs] = tuple;
      return linear_combination(lhs, 1.0f - t) + linear_combination(rhs, t);
    });
  
  return Matrix<R>{std::ranges::to<std::vector<R> >(interpolation)};
}
