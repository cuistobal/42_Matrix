#pragma once

#include <vector>
#include <string>
#include <format>
#include <ostream>
#include <iostream>

template <typename T>
class Matrix {
  public:

//    using Matrix = std::vector<std::vector<T> >;
//    using matrix = std::vector<std::vector<T> >;
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

  private:
    size_t _rows{0};
    size_t _cols{0};
    std::vector<std::vector<T> > _data{};
};

#include "Matrix.tpp"
