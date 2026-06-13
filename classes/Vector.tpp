/* Constructors */

template <vector::Numeric T>
Vector<T>::Vector(std::initializer_list<T> data)
    : _dimensions(data.size()),
      _data(data) {
}

template <vector::Numeric T>
template <typename R>
requires vector::flat_range<R, T>
Vector<T>::Vector(R&& data)
  : _dimensions(std::ranges::size(data)),
    _data(std::ranges::begin(data), std::ranges::end(data)) {
}

template <vector::Numeric T>
size_t Vector<T>::get_dimensions() const {
  return _dimensions;
}

template <vector::Numeric T>
const std::vector<T>& Vector<T>::get_data() const {
  return _data;
}

/* Operator overloads*/

template <vector::Numeric T> 
template <vector::Numeric U>
vector::PVector<T, U> Vector<T>::operator+(
  const Vector<U>& other) const {
  vector::PVector<T, U> result(*this);
  const auto& data{other.get_data()};
  for (const auto& p: data) {
    result._data += p; 
  }
  return result;
}

template <vector::Numeric T> 
template <vector::Numeric U>
Vector<T>& Vector<T>::operator+=(
  const Vector<U>& other) {
  const auto& data{other.get_data()};

  for (size_t i{0uz}; i < data.size(); ++i) {
    _data[i] += data[i];
  }

  return *this;
}

template <vector::Numeric T>
template <vector::Numeric U>
vector::PVector<T, U> Vector<T>::operator-(
  const Vector<U>& other) const {
  vector::PVector<T, U> result(*this);
  const auto& data{other.get_data()};
  for (const auto& p: data) {
    result._data -= p; 
  }
  return result;
}

template <vector::Numeric T>
template <vector::Numeric U>
vector::PVector<T, U> Vector<T>::operator*(
  const Vector<U>& other) const {
  vector::PVector result(*this);
  const auto& data{other.get_data()};
  for (const auto& d : data) {
    _data *= d;
  }
  return result;
}

// La bonne question etant, est-ce qu on promote/demote le vecteur en fonction 
// de son scalaire
template <vector::Numeric T>
template <vector::Numeric U>
vector::PVector<T, U> Vector<T>::operator*(
  const U& scalar) const {
  using R = vector::promoted_type<T, U>;
  std::vector<R> newData(_data);
  for (size_t i{0uz}; i < _data.size(); ++i) {
    newData[i] *= scalar;
  }
  return {newData};
}

/* Formatter */

template <vector::Numeric T>
struct std::formatter<Vector<T>> {
  std::string element_spec{"{}"};

  constexpr auto parse(std::format_parse_context& ctx) {
    auto it = ctx.begin();
    auto end = ctx.end();

    if (it != end && *it != '}') {
      element_spec = "{:";
      while (it != end && *it != '}') {
        element_spec += *it++;
      }
      element_spec += "}";
    }

    return it;
  }

  auto format(const Vector<T>& v, std::format_context& ctx) const {
    auto out{ctx.out()};
    const auto& data = v.get_data();

    out = std::format_to(out, "[ ");

    for (size_t i = 0; i < data.size(); ++i) {
      out = std::vformat_to(out, element_spec, std::make_format_args(data[i]));

      if (i + 1 < data.size()) {
        out = std::format_to(out, ", ");
      }
    }

    return std::format_to(out, " ]\n");
  }
};
