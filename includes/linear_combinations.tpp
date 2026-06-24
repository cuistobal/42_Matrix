#include <cassert>

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] inline concepts::Promoted_Type<T, U> linear_combination(
  T multiplicand,  
  U scalar) noexcept {
  using R = concepts::Promoted_Type<T, U>;
  return static_cast<R>(multiplicand * scalar);
}

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] Vector<concepts::Promoted_Type<T, U>> linear_combination(
  std::vector<Vector<T>>& candidates, 
  std::vector<U>& scalars) noexcept {

  assert(!candidates.empty() && !scalars.empty() && "This function requires candidates and scalars");

  size_t expected_size{candidates[0].get_dimensions()};
  
  assert(scalars.size() == candidates.size() && "Scalars and candidates count mismatch");

  Vector<concepts::Promoted_Type<T, U>> ans = candidates[0] * scalars[0];

  for (size_t i{1uz}; i < scalars.size(); ++i) {
    assert(candidates[i].get_dimensions() == expected_size && "Candidate size mismatch");
    ans += linear_combination(candidates[i], scalars[i]);
  }

  return ans;
}
