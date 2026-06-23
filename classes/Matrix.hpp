#pragma once

#include <string>
#include <vector>
#include <ranges>
#include <format>
#include <ostream>
#include <concepts>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <initializer_list>
// Can't use the offcial lib for some reason (not present on ym system atm :()

namespace matrix {
  // Faire evoluer le concept Numeric en Scalar pour supporter la gestion des 
  // nombres complexes
  template <typename T>
  concept Numeric = std::integral<T> || std::floating_point<T>;

  template <Numeric T, Numeric U>
  using promoted_type = std::common_type_t<T, U>;

  template <typename R, typename T>
  concept nested_range =
    Numeric<T> &&
    std::ranges::forward_range<R> &&
    std::ranges::forward_range<std::ranges::range_value_t<R>> &&
    std::convertible_to<
      std::ranges::range_value_t<
        std::ranges::range_value_t<R>>, T>;

  template <typename R, typename T>
  concept flat_range =
    Numeric<T> &&
    std::ranges::contiguous_range<R> &&
    std::convertible_to<
      std::ranges::range_value_t<R>, T>;
}

template <matrix::Numeric T>
class Matrix;

namespace matrix {
  template <Numeric T, Numeric U>
  using PMatrix = Matrix<promoted_type<T, U>>;
}

template <matrix::Numeric T>
class Matrix {
  public:

    using Dataset = std::initializer_list<std::initializer_list<T>>;

    Matrix() = delete;
  
    template <typename R>
    requires matrix::flat_range<R, T>
    Matrix(R&& data, size_t row, size_t col);

    template <typename R>
    requires matrix::nested_range<R, T>
    Matrix(R&& data);
    Matrix(Dataset data);
    Matrix(Matrix<T>&& other) = default;
    Matrix(const Matrix<T>& other) = default;
    template <matrix::Numeric U>
    Matrix(const Matrix<U>& other);
    ~Matrix() = default;

    size_t get_rows() const noexcept;
    size_t get_cols() const noexcept;
    const std::vector<T>& get_data() const noexcept;
    std::pair<size_t, size_t> get_shape() const noexcept;

    template <matrix::Numeric U>
    [[nodiscard]]
    matrix::PMatrix<T, U> operator+(const Matrix<U>& other) const;

    template <matrix::Numeric U>
    Matrix<T>& operator+=(const Matrix<U>& other);

    template <matrix::Numeric U>
    [[nodiscard]]
    matrix::PMatrix<T, U> operator-(const Matrix<U>& other) const;

    template <matrix::Numeric U>
    Matrix<T>& operator-=(const Matrix<U>& other);

    template <matrix::Numeric U>
    [[nodiscard]]
    matrix::PMatrix<T, U> operator*(const Matrix<U>& other) const;

    template <matrix::Numeric U>
    [[nodiscard]]
    matrix::PMatrix<T, U> operator*(const U& scalar) const;

    template <matrix::Numeric U>
    Matrix<T>& operator*=(const U& scalar);

    friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& src) {
      return os << std::format("{}", src);
    }

  private:
    size_t _rows{0};
    size_t _cols{0};
    std::vector<T> _data{};
};

template <matrix::Numeric T, matrix::Numeric U>
auto operator<=>(const Matrix<T>& lhs, const Matrix<U>& rhs);

template <matrix::Numeric T, matrix::Numeric U>
auto operator==(const Matrix<T>& lhs, const Matrix<U>& rhs);

template <typename T>
Matrix(std::initializer_list<std::initializer_list<T>>) -> Matrix<T>;

#include "Matrix.tpp"
