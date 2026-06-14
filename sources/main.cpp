#include "ft_matrix.hpp"

// Exercise01
template <vector::Numeric T>
static Vector<T> linear_combination(
  std::vector<Vector<T>>& candidates, 
  std::vector<T>& scalars) {

  size_t expected_size{scalars.size()};

  if (candidates.size() != expected_size) {
    throw 
      std::invalid_argument(
        "Scalars and candidate sizes mismatch");
  }

  Vector<T> ans(candidates[0] * scalars[0]);

  for (size_t i{1uz} ; i < scalars.size(); ++i) {
    if (candidates[i].get_dimensions() != expected_size) {
      throw 
        std::invalid_argument(
          "Candidate size mismatch");
    } else {
      ans += candidates[i] * scalars[i];
    }
  }

  return ans;
}

int main() {

//  testing::InitGoogleTest();
//
//  return RUN_ALL_TESTS();
//}

  Matrix<int> m{{{-1, 2}, {3, 4}}};
  std::println("{}", m);

  Matrix<unsigned int> n{{{1, 2}, {3, 4}}};
  std::println("{}", n);

  std::println("m == n ? {}\n", m == n);

  std::println("{}", m + n);

  Vector<int> v{1, 2, 3};
  std::println("{}", v);

  Matrix<int> a2{{1, 2, 3}, {4, 5, 6}};
  Matrix<int> a1{{1, 2}, {0, 1}, {2, 3}};

  std::println("multiplications: ");
  std::println("{}", a1 * a2);
  std::println("{}", m * n);


  std::vector<int> scalars(v.get_data());
  std::vector<Vector<int>> candidates;

  candidates.push_back(v);
  candidates.push_back(v);
  candidates.push_back(v);
  
  Complex c(2.3, 3.2);

//  std::print("Candidates -> "); 
//  for (const auto& c:candidates) {
//    std::print("{}", c); 
//  }
//  std::println("");
//  std::println("Scalars -> {}", scalars); 
  std::println("Linear combination -> {}", linear_combination(candidates, scalars));

  return 0;
}
