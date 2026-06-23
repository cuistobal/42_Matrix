#include "ft_matrix.hpp"


//template <vector::promoted_type T, vector::Numeric U>

// The linear combination is the sum of the product of n vectors by n scalars
template <vector::Numeric T>
Vector<T> linear_combination(
  std::vector<Vector<T>>& candidates, 
  std::vector<T>& scalars) {

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

  Vector<T> ans = candidates[0] * scalars[0];

  for (size_t i{1uz}; i < scalars.size(); ++i) {
    if (candidates[i].get_dimensions() != expected_size) {
      throw 
        std::invalid_argument(
          "Candidate size mismatch");
    }
    ans += candidates[i] * scalars[i];
  }

  return ans;
}
