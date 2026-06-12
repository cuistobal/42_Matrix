/* Constructors */

template <vector::Numeric T>
Vector<T>::Vector(std::initializer_list<T> data)
    : _dimensions(data.size()),
      _data(data) {
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
  std::vector<T> newData(_data);
  for (size_t i{0uz}; i < _data.size(); ++i) {
    newData[i] *= scalar;
  }
  return Vector<T>(newData);
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
