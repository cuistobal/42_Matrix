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

  return 0;
}
