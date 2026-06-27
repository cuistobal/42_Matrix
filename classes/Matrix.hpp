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

template <concepts::Numeric T>
class Matrix {
  public:

    using Dataset = std::initializer_list<std::initializer_list<T>>;

    Matrix() = delete;
  
    template <typename R>
    requires concepts::flat_range<R, T>
    Matrix(R&& data, size_t row, size_t col);

    template <typename R>
    requires concepts::nested_range<R, T>
    Matrix(R&& data);

    Matrix(Dataset data);
    Matrix(Matrix<T>&& other) = default;
    Matrix(const Matrix<T>& other) = default;

    template <concepts::Numeric U>
    Matrix(const Matrix<U>& other);

    ~Matrix() = default;

    size_t get_rows() const noexcept;
    size_t get_cols() const noexcept;
    const std::vector<T>& get_data() const noexcept;
    std::pair<size_t, size_t> get_shape() const noexcept;

    template <concepts::Numeric U>
    Matrix<T>& operator+=(const Matrix<U>& other);

    template <concepts::Numeric U>
    Matrix<T>& operator-=(const Matrix<U>& other);

    template <concepts::Numeric U>
    Matrix<T>& operator*=(const U& scalar);

    auto operator<=>(const Matrix<T>& other) const = default;

    friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& src) {
      return os << std::format("{}", src);
    }

  private:
    size_t _rows{0};
    size_t _cols{0};
    std::vector<T> _data{};
};

template <concepts::Numeric T, concepts::Numeric U>
auto operator<=>(const Matrix<T>& lhs, const Matrix<U>& rhs);

template <concepts::Numeric T, concepts::Numeric U>
auto operator==(const Matrix<T>& lhs, const Matrix<U>& rhs);

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator+(const Matrix<T>& lhs, const Matrix<U>& rhs) -> Matrix<concepts::Promoted_Type<T, U>>;

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator-(const Matrix<T>& lhs, const Matrix<U>& rhs) -> Matrix<concepts::Promoted_Type<T, U>>;

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator*(const Matrix<T>& lhs, const Matrix<U>& rhs) -> Matrix<concepts::Promoted_Type<T, U>>;

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator*(const Matrix<T>& lhs, const U& scalar) -> Matrix<concepts::Promoted_Type<T, U>>;

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator*(const T& scalar, const Matrix<U>& rhs) -> Matrix<concepts::Promoted_Type<T, U>>;

template <typename T>
Matrix(std::initializer_list<std::initializer_list<T>>) -> Matrix<T>;

#include "Matrix.tpp"
