#include <print>

#include "Matrix.hpp"
#include "Vector.hpp"

int main() {

  Matrix<int> m{{{1, 2}, {3, 4}}};
  std::print("{}", m);

  Vector<int> v{1, 2, 3};
  std::print("{}", v);

  return 0;
}
