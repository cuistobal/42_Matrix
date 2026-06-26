#include "ft_matrix.hpp"

template <concepts::Numeric t, concepts::Numeric U>
auto dot(
  const Vector<T>& lhs,
  const Vector<U>& rhs) 
  -> Vector<concepts::promoted_type<T, U> > {
  using R = concepts::promoted_type<T,U>;
  
  auto dot = 
    std::views::zip() | 
    std::views::tranform();
  
  return Vector<R>{};
}

template <concepts::Numeric t, concepts::Numeric U>
auto dot(
  const Matrix<T>& lhs,
  const Matrix<U>& rhs) 
  -> Matrix<concepts::promoted_type<T, U> > {
  using R = concepts::promoted_type<T,U>;

  

  return Matrix<R>{}
}
