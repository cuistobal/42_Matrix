#pragma once

template <concepts::Numeric T>
class Complex {
  public:

    Complex() = delete;

    template <concepts::Numeric U>
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

template <concepts::Numeric T, concepts::Numeric U>
auto operator <=>(const Complex<T>& lhs, const Complex<U>& rhs);

template <concepts::Numeric T, concepts::Numeric U>
auto operator ==(const Complex<T>& lhs, const Complex<U>& rhs);

#include "Complex.tpp"
