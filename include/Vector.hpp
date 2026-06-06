#pragma once

template <typename T>
class Vector<T> {
  public:

    Vector() = delete;

    Vector(const T& x, const T& y, const T& z);
    Vector(const Vector& other);
    
    ~Vector();

    auto operator<=>() const = default;

    size() const;

    friend ostream& operator<<(std::ostream& os, const Vector<T>& src);

  private:
    T _x;
    T _y;
    T _z;
};
