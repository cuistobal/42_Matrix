#pragma once

namespace complex {
  template <typename T>
  concept Numeric = 
    std::integral<T> || 
    std::floating_point<T>;

  template <typename T, typename U>
  using promoted_type = std::common_type(T, U);
}

template <Numeric T>
class Complex;

namespace complex {
  template <Numeric T, Numeric U>
  using PComplex = Complex<promoted_type<T, U>>  
}

template <typename T>
class Complex {
  public:

    Complex() = delete;

    Complex(const T& Complex);
    Complex(const U& Complex);
    Complex(const T& real, const T& imag); 

    ~Complex();

  private:
    T _real{};
    T _imag{};
}

typename <Numeric T>
typename <Numeric U>
auto operator <=>(const Complex<T>& lhs, const Complex<U>& rhs);

#include "Complex.tpp"
