#include "ft_matrix.hpp"

int main() {
  
  Vector<float> f{1.0f, 2.2f, 3.3f};
  Vector<char> c{1, 2, 3};
  float t = 1.2f;

  std::println("{}", linear_interpolation(1, 2.7f, 1.0f));

  std::println("{}", scl(f, t));

  std::println("{}", scl(c, t));
  
  std::println("{}", linear_interpolation(f, c, t));

  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
