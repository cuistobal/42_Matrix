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

namespace matrix {
  template <typename T>
  concept Numeric = std::integral<T> || std::floating_point<T>;

  template <Numeric T, Numeric U>
  using promoted_type = std::common_type_t<T, U>;
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
    Matrix(Dataset data);
    Matrix(const Matrix<T>& other) = default;
    ~Matrix() = default;

    size_t get_rows() const;
    size_t get_cols() const;
    const Grid& get_data() const;

    template <matrix::Numeric U>
    [[nodiscard]]
    auto operator<=>(const Matrix<U>& other) const;

//    auto operator<=>(const Matrix<T>& other) const = default;

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
    Grid _data{};
};

#include "Matrix.tpp"
