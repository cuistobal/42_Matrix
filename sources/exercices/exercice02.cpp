#include "ft_matrix.hpp"

/*
 * LERP FORMULA
 * lerp(a,b,t)=a+t×(b−a)
 * linear_combination(u, 1 - t) + linear_combination(v, t);
 */

//template <concepts::Numeric T, concepts::Numeric U>
//inline concepts::Promoted_Type<T, U> 
//  linear_interpolation(
//    const T& u, 
//    const U& v, 
//    std::float32_t t) {
//  return 
//    static_cast<R>(
//      linear_combination(u, t - 1) + 
//      linear_combination(v, t)); 
//}
//
//template <vector::Numeric T, vector::Numeric U>
//vector::PVector<T, U> linear_interpolation(
//  const Vector<T>& u,
//  const Vector<U>& v,
//  std::float32_t t) { 
//  
//  using R = vector::promoted_type<T, U>;
//
//  std::vector<R> data;
//
//  return Vector<R>(data);
//}
//
//template <matrix::Numeric T, matrix::Numeric U>
//matrix::PMatrix<T, U> linear_interpolation(
//  const Matrix<T>& u,
//  const Matrix<U>& v,
//  std::float32_t t) { 
//
////  matrix::Shape ushape = u.get_shape();
////  matrix::Shape vshape = v.get_shape();
//
//  if (u.get_shape() != v.get_shape()) {
//    throw
//      std::invalid_argument(
//        "Can't interpolate matrixes with different shapes");
//  }
//  
//
//  using R = matrix::promoted_type<T, U>;
//
//  std::vector<R> data;
//
//  return Matrix<R>(data);
//}
