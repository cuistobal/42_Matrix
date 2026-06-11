#include "ft_matrix.hpp"

int main() {

  Matrix<int> m{{{1, 2}, {3, 4}}};
  std::print("{}", m);

  Matrix<int> n{{{1, 2}, {3, 4}}};
  std::print("{}", m);

  std::print("{}", m + n);

  Vector<int> v{1, 2, 3};
  std::print("{}", v);

  return 0;
}
