template <concepts::Numeric T, concepts::Numeric U>
inline concepts::Promoted_Type<T, U> linear_combination(
  T multiplicand,  
  U scalar) {
  using R = concepts::Promoted_Type<T, U>;
  return static_cast<R>(multiplicand * scalar);
}

template <concepts::Numeric T, concepts::Numeric U>
vector::PVector<T, U> linear_combination(
  std::vector<Vector<T>>& candidates, 
  std::vector<U>& scalars) {

  if (candidates.empty() || scalars.empty()) {
    throw 
      std::invalid_argument(
        "This function requires candidates and scalars");
  }

  size_t expected_size{candidates[0]};
  if (scalars.size() != expected_size) {
    throw 
      std::invalid_argument(
        "Scalars and candidates size mismatch");
  }

  vector::PVector<T, U> ans = candidates[0] * scalars[0];

  for (size_t i{1uz}; i < scalars.size(); ++i) {
    if (candidates[i].get_dimensions() != expected_size) {
      throw 
        std::invalid_argument(
          "Candidate size mismatch");
    }
//    ans += candidates[i] * scalars[i];
    ans += linear_combination(candidates[i], scalars[i]);
  }

  return ans;
}
