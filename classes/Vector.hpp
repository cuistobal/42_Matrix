#pragma once

#include <vector>
#include <ostream>
#include <concepts>
#include <type_traits>

namespace vector {
  template <typename T>
  concept Numeric = std::integral<T> || std::floating_point<T>;
}

template <vector::Numeric T>
class Vector {
  public:

    Vector() = delete;
    Vector(std::initializer_list<T> data);
    Vector(const Vector& other) = default; 
    ~Vector() = default;

    auto operator<=>(const Vector& other) const = default;

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
