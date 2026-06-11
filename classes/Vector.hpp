#pragma once

#include <vector>
#include <ostream>
#include <concepts>
#include <type_traits>

namespace vector {
  template <typename T>
  concept Numeric = std::integral<T> || std::floating_point<T>;

  template <Numeric T, Numeric U>
  using promoted_type = std::common_type_t<T, U>;

}

template <vector::Numeric T>
class Vector;

namespace vector {
  template <vector::Numeric U, vector::Numeric T>
  using PVector = Vector<matrix::promoted_type<T, U> >;
}

template <vector::Numeric T>
class Vector {
  public:

    Vector() = delete;
    Vector(std::initializer_list<T> data);
    Vector(const Vector& other) = default; 
    ~Vector() = default;

    template <vector::Numeric U>
    auto operator<=>(const Vector<U>& other) const;

//    auto operator<=>(const Vector<T>& other) const = default;

//    [[nodiscard]]
//    template <vector::Numeric U>
//    vector::PVector<T, U> operator+(const Vector<U>& other) const;
//
//    template <vector::Numeric U>
//    Vector<T>& operator+=(const Vector<U>& other);
//
//    [[nodiscard]]
//    template <vector::Numeric U>
//    vector::PVector<T, U> operator+(const Vector<U>& other) const;
//
//    template <vector::Numeric U>
//    Vector<T>& operator+=(const Vector<U>& other);
//
//    [[nodiscard]]
//    template <vector::Numeric U>
//    vector::PVector<T, U> operator+(const Vector<U>& other) const;
//
//    template <vector::Numeric U>
//    Vector<T>& operator+=(const Vector<U>& other);
//
//    [[nodiscard]]
//    template <vector::Numeric U>
//    vector::PVector<T, U> operator+(const Vector<U>& other) const;
//
//    template <vector::Numeric U>
//    Vector<T>& operator+=(const Vector<U>& other);

    size_t get_dimensions() const;
    const std::vector<T>& get_data() const;

    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& src) {
      return os << src;
    }

  private:
    size_t _dimensions;
    std::vector<T> _data;
};

#include "Vector.tpp"
