#include <cassert>

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] inline concepts::Promoted_Type<T, U> linear_interpolation(const T& u, const U& v, std::float32_t t) noexcept {
  using R = concepts::Promoted_Type<T, U>;
  return 
    static_cast<R>(
      linear_combination(u, 1.0f - t) + 
      linear_combination(v, t));
}

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] Vector<concepts::Promoted_Type<T, U>> linear_interpolation(const Vector<T>& u, const Vector<U>& v, std::float32_t t) noexcept { 
  
  assert(u.get_dimensions() == v.get_dimensions() && "Can't interpolate vectors with different shapes");

  using R = concepts::Promoted_Type<T, U>;

  auto interpolation = 
    std::views::zip(u.get_data(), v.get_data()) |
    std::views::transform([t](auto&& tuple) {
      auto&& [lhs, rhs] = tuple; 
      return linear_combination(lhs, 1.0f - t) + linear_combination(rhs, t);
    }
  );

  return Vector<R>{std::ranges::to<std::vector<R>>(interpolation)};
}

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] Matrix<concepts::Promoted_Type<T, U>> linear_interpolation(const Matrix<T>& u, const Matrix<U>& v, std::float32_t t) noexcept { 

  assert(u.get_shape() == v.get_shape() && "Can't interpolate matrixes with different shapes");

  using R = concepts::Promoted_Type<T, U>;

  auto interpolation = 
    std::views::zip(u.get_data(), v.get_data()) |
    std::views::transform([t](auto&& tuple) {
      auto&& [lhs, rhs] = tuple;
      return linear_combination(lhs, 1.0f - t) + linear_combination(rhs, t);
    });

  return Matrix<R>(
    std::ranges::to<std::vector<R>>(interpolation),
    u.get_rows(),
    u.get_cols());
}
