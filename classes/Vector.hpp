#pragma once

#include <ranges>
#include <vector>
#include <ostream>
#include <concepts>
#include <type_traits>

namespace vector {
  template <typename T>
  concept Numeric = std::integral<T> || std::floating_point<T>;

  template <Numeric T, Numeric U>
  using promoted_type = std::common_type_t<T, U>;

  template <typename R, typename T>
  concept flat_range = 
    std::ranges::contiguous_range<R> &&
    std::convertible_to<
      std::ranges::range_value_t<R>, T>;
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

    template <typename R>
    requires vector::flat_range<R, T>
    Vector(R&& data);

    template <vector::Numeric U>
    Vector(const Vector<U>& other);

    Vector(std::initializer_list<T> data);
    Vector(const Vector<T>& other) = default; 
    ~Vector() = default;

    template <vector::Numeric U>
    auto operator<=>(const Vector<U>& other) const;

    template <vector::Numeric U>
    [[nodiscard]]
    vector::PVector<T, U> operator+(const Vector<U>& other) const;

    template <vector::Numeric U>
    Vector<T>& operator+=(const Vector<U>& other);

    template <vector::Numeric U>
    [[nodiscard]]
    vector::PVector<T, U> operator-(const Vector<U>& other) const;

    template <vector::Numeric U>
    Vector<T>& operator-=(const Vector<U>& other);

    template <vector::Numeric U>
    [[nodiscard]]
    vector::PVector<T, U> operator*(const Vector<U>& other) const;

    template <vector::Numeric U>
    Vector<T>& operator*=(const Vector<U>& other);

    template <vector::Numeric U>
    [[nodiscard]]
    vector::PVector<T, U> operator*(const U& scalar) const;

    template <vector::Numeric U>
    Vector<T>& operator*=(const U& scalar);

    size_t get_dimensions() const;
    const std::vector<T>& get_data() const;

    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& src) {
      return os << src;
    }

  private:
    size_t _dimensions;
    std::vector<T> _data;
};

template <vector::Numeric T, vector::Numeric U>
auto operator<=>(const Vector<T>& lhs, const Vector<U>& rhs);

template <vector::Numeric T, vector::Numeric U>
auto operator==(const Vector<T>& lhs, const Vector<U>& rhs);

#include "Vector.tpp"
