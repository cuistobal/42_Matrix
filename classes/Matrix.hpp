#pragma once

#include <concepts>
#include <format>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include <ranges>

namespace matrix {
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
    using Row = std::vector<T>;
    using Grid = std::vector<Row>;
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

    size_t get_rows() const;
    size_t get_cols() const;
    const std::vector<T>& get_data() const;
//    const Grid& get_data() const;

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
    Matrix<T>& operator*=(const Matrix<U>& other);

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
//    Grid _data{};
    std::vector<T> _data{};
};

template <matrix::Numeric T, matrix::Numeric U>
[[nodiscard]]
auto operator<=>(const Matrix<T>& lhs, const Matrix<U>& rhs);

template <matrix::Numeric T, matrix::Numeric U>
[[nodiscard]]
auto operator==(const Matrix<T>& lhs, const Matrix<U>& rhs);

template <matrix::Numeric T, matrix::Numeric U>
[[nodiscard]]
auto operator!=(const Matrix<T>& lhs, const Matrix<U>& rhs);

// Tout en bas de Matrix.hpp
template <typename T>
Matrix(std::initializer_list<std::initializer_list<T>>) -> Matrix<T>;

#include "Matrix.tpp"
