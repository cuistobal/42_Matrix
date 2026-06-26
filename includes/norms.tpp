// Manhattan norm
template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] std::float32_t norm_1(const Vector<T>& lhs, const Vector<U>& rhs) -> concepts::Promoted_Type<T, U> {

}

// Euclidian norm
template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] std::float32_t norm(const Vector<T>& lhs, const Vector<U>& rhs) -> concepts::Promoted_Type<T, U> {

  assert(lhs.get_dimensions() == rhs.get_dimensions && "Can't compute euclidian norm of vectors with different shapes");

  std::float32_t norm{0.0f};

  auto euclidian = 
    std::views::zip(lhs.get_data(), rhs.get_data()) |
    std::views::transform([](auto&& tuple){
      auto&& [lhs, rhs] = tuple;
      return std::abs(std::abs(lhs) - std::abs(rhs));
    })
  return std::accumulate(euclidian.begin(), euclidian.end(), ret); 
}

// Supremum norm
template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] std::float32_t norm_inf(const Vector<T>& lhs, const Vector<U>& rhs) -> concepts::Promoted_Type<T, U> {

}
