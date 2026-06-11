#include "ft_matrix.hpp"

int main() {

  Matrix<int> m{{{-1, 2}, {3, 4}}};
  std::println("{}", m);

  Matrix<unsigned int> n{{{1, 2}, {3, 4}}};
  std::println("{}", m);

  std::println("m == n ? {}\n", m == n);

  std::println("{}", m + n);

  Vector<int> v{1, 2, 3};
  std::println("{}", v);

  Matrix<int> a2{{1, 2, 3}, {4, 5, 6}};
  Matrix<int> a1{{1, 2}, {0, 1}, {2, 3}};

  std::println("Multiplications: ");
  std::println("{}", a1 * a2);

  return 0;
}
