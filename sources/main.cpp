#include "ft_matrix.hpp"

static void exercise00() {

  // Vector addition

  // Vector substractions

  // Vector scaling

}

int main() {

  exercise00();

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

  std::println("Multiplications: ");
  std::println("{}", a1 * a2);
  std::println("{}", m * n);

  return 0;
}
