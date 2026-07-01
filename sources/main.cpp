#include "ft_matrix.hpp"

int main() {
  Matrix<int> m{{1,2,3},{4,5,6},{7,8,9}};
  std::println("{}", m);
  Matrix<int> ref = m.row_echelon_form();
  std::println("{}", ref);
  std::println("{}", ref.determinant());

  Matrix<int> n{{3,6,3},{41,-25,6},{7,-8,1}};
  std::println("{}", n);
  Matrix<int> r = n.row_echelon_form();
  std::println("{}", r);
  std::println("{}", r.determinant());

  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
