#pragma once

#include <ranges>
#include <vector>
#include <ostream>
#include <concepts>
#include <type_traits>

template <concepts::Numeric T>
class Vector {
  public:
    Vector() = delete;

    template <typename R>
    requires concepts::flat_range<R, T>
    Vector(R&& data);

    template <concepts::Numeric U>
    Vector(const Vector<U>& other);

    Vector(std::initializer_list<T> data);
    Vector(const Vector<T>& other) = default; 
    ~Vector() = default;

    template <concepts::Numeric U>
    [[nodiscard]]
    Vector<concepts::Promoted_Type<T, U>> operator+(const Vector<U>& other) const;

    template <concepts::Numeric U>
    Vector<T>& operator+=(const Vector<U>& other);

    template <concepts::Numeric U>
    [[nodiscard]]
    Vector<concepts::Promoted_Type<T, U>> operator-(const Vector<U>& other) const;

    template <concepts::Numeric U>
    Vector<T>& operator-=(const Vector<U>& other);

    template <concepts::Numeric U>
    [[nodiscard]]
    Vector<concepts::Promoted_Type<T, U>> operator*(const Vector<U>& other) const;

    template <concepts::Numeric U>
    Vector<T>& operator*=(const Vector<U>& other);

    template <concepts::Numeric U>
    [[nodiscard]]
    Vector<concepts::Promoted_Type<T, U>> operator*(const U& scalar) const;

    template <concepts::Numeric U>
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

template <concepts::Numeric T, concepts::Numeric U>
auto operator<=>(const Vector<T>& lhs, const Vector<U>& rhs);

template <concepts::Numeric T, concepts::Numeric U>
auto operator==(const Vector<T>& lhs, const Vector<U>& rhs);

#include "Vector.tpp"
