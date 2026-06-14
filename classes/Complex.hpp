#pragma once

namespace complex {
  template <typename T>
  concept Numeric = 
    std::integral<T> || 
    std::floating_point<T>;

  template <Numeric T, Numeric U>
  using promoted_type = std::common_type_t<T, U>;
}

template <complex::Numeric T>
class Complex;

namespace complex {
  template <Numeric T, Numeric U>
  using PComplex = Complex<promoted_type<T, U>>;
}

template <complex::Numeric T>
class Complex {
  public:

    Complex() = delete;

    template <complex::Numeric U>
    Complex(const Complex<U>& other);
    Complex(const T& real, const T& imag); 

    ~Complex() = default;
    Complex(Complex<T>&& other) = default;
    Complex(const Complex<T>& other) = default;

    T get_real() const;
    T get_imag() const;

  private:
    T _real{};
    T _imag{};
};

template <complex::Numeric T, complex::Numeric U>
auto operator <=>(const Complex<T>& lhs, const Complex<U>& rhs);

template <complex::Numeric T, complex::Numeric U>
auto operator ==(const Complex<T>& lhs, const Complex<U>& rhs);

#include "Complex.tpp"
