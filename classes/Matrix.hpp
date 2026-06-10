#pragma once

#include <vector>
#include <string>
#include <format>
#include <ostream>
#include <iostream>
#include <concepts>
#include <type_traits>

template <typename T>
//concept Numeric = std::integral<T> || std::floating_point<T>;
class Matrix {
  public:

    using Dataset = std::initializer_list<std::initializer_list<T> >;

    Matrix() = delete;
    Matrix(Dataset data);
    Matrix(const Matrix<T>& other) = default;
    ~Matrix() = default;

    size_t get_rows() const;
    size_t get_cols() const;
    const std::vector<std::vector<T> >& get_data() const;

    auto operator <=>(const Matrix<T>& other) const = default;

    friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& src) {
      return os << std::format("{}", src);
    };

    // Exercice 00
//    add(const Matrix<U>& other);
//    substract(const Matrix<U>& other);
//    scale(const U& scalar);
//
//    add(const Matrix<U>& other);
//    substract(const Matrix<U>& other);
//    scale(const U& scalar);

  private:
    size_t _rows{0};
    size_t _cols{0};
    std::vector<std::vector<T> > _data{};
};

#include "Matrix.tpp"
