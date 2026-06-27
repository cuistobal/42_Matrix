template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto dot(
  const Vector<T>& lhs, 
  const Vector<U>& rhs) 
  -> concepts::Promoted_Type<T, U> {
  
  assert(lhs.get_dimensions() == rhs.get_dimensions() && "Unable to compute dot product of vectors with different sizes");

  using R = concepts::Promoted_Type<T, U>;

  auto dot_product = 
    std::views::zip(lhs.get_data(), rhs.get_data()) | 
    std::views::transform([](auto&& tuple){
      auto&& [lhs, rhs] = tuple;
      return lhs * rhs;
    }); 

  return std::accumulate(dot_product.begin(), dot_product.end(), R{0});
};

template <concepts::Numeric T, concepts::Numeric U>
auto dot(
  const Matrix<T>& lhs, 
  const Matrix<U>& rhs) 
  -> concepts::Promoted_Type<T, U> {
  
  assert(rhs.get_shape() == lhs.get_shape() && "Unable to compute dot product of matrixes with different sizes");

  using R = concepts::Promoted_Type<T, U>;

  auto dot_product = 
    std::views::zip(lhs.get_data(), rhs.get_data()) |
    std::views::transform([](auto&& tuple){
      auto&& [rhs, lhs] = tuple;
      return rhs * lhs;});

  return std::accumulate(dot_product.begin(), dot_product.end(), R{0});
}
