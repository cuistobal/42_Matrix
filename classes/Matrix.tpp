/* Ctors && Dtors */

template <matrix::Numeric T>
Matrix<T>::Matrix(Dataset data) {
  for (const auto& row: data) {
    _data.push_back(std::vector<T>(row));
  }
  _rows = _data.size();
  _cols = _data.empty() ? 0 : _data[0].size();
} 

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>::Matrix(const Matrix<U>& other)
  : _rows(other.get_rows()),
  _cols(other.get_cols()),
  _data(_rows, std::vector<T>(_cols)) {
  const auto& src = other.get_data();

  for (size_t i = 0; i < _rows; ++i) {
    for (size_t j = 0; j < _cols; ++j) {
      _data[i][j] = static_cast<T>(src[i][j]);
    }
  }
}

template <matrix::Numeric T>
size_t Matrix<T>::get_rows() const {
    return _rows;
}

template <matrix::Numeric T>
size_t Matrix<T>::get_cols() const {
    return _cols;
}

template <matrix::Numeric T>
const std::vector<std::vector<T> >& Matrix<T>::get_data() const {
    return _data;
}

/* Operator overloads */

template <matrix::Numeric T>
template <matrix::Numeric U>
matrix::PMatrix<T, U>
Matrix<T>::operator+(const Matrix<U>& other) const {
  matrix::PMatrix<T, U> tmp{*this};
  tmp += other;
  return tmp;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>& Matrix<T>::operator+=(const Matrix<U>& other) {
  const auto& data{other.get_data()};
  for (size_t i{0}; i < data.size(); ++i) {
    for (size_t j{0}; j < data[i].size(); ++j) {
      this->_data[i][j] += data[i][j];
    }
  }
  return *this;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
matrix::PMatrix<T, U>
Matrix<T>::operator-(const Matrix<U>& other) const {
  matrix::PMatrix<T, U> tmp{*this};
  tmp -= other;
  return tmp;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>& Matrix<T>::operator-=(const Matrix<U>& other) {
  const auto& data{other.get_data()};
  for (size_t i{0}; i < _data.size(); ++i) {
    for (size_t j{0}; j < _data[i].size(); ++j) {
      _data[i][j] -= data[i][j];
    }
  }
  return *this;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
matrix::PMatrix<T, U>
Matrix<T>::operator*(const Matrix<U>& other) const {
  matrix::PMatrix<T, U> tmp{*this};
  tmp *= other;
  return tmp;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>& Matrix<T>::operator*=(const Matrix<U>& other) {
  const auto& data{other.get_data()};
  for (size_t i{0}; i < _data.size(); ++i) {
    for (size_t j{0}; j < _data[i].size(); ++j) {
      _data[i][j] *= data[i][j];
    }
  }
  return *this;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
matrix::PMatrix<T, U>
Matrix<T>::operator*(const U& scalar) const {
  matrix::PMatrix<T, U> tmp{*this};
  tmp *= scalar;
  return tmp;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>& Matrix<T>::operator*=(const U& scalar) {
  for (size_t i{0}; i < _data.size(); ++i) {
    for (size_t j{0}; j < _data[i].size(); ++j) {
      _data[i][j] *= scalar;
    }
  }
  return *this;
}

template <matrix::Numeric T, matrix::Numeric U>
[[nodiscard]]
auto operator<=>(const Matrix<T>& lhs, const Matrix<U>& rhs) {
  if (auto cmp = lhs.get_rows() <=> rhs.get_rows(); cmp != 0) return cmp;
  if (auto cmp = lhs.get_cols() <=> rhs.get_cols(); cmp != 0) return cmp;
  return lhs.get_data() <=> rhs.get_data();
}

template <matrix::Numeric T, matrix::Numeric U>
[[nodiscard]]
auto operator==(const Matrix<T>& lhs, const Matrix<U>& rhs) {
  if constexpr (std::same_as<T, U>) {
    return lhs.get_rows() == rhs.get_rows() &&
      lhs.get_cols() == rhs.get_cols() &&
      lhs.get_data() == rhs.get_data();
  }
  return false;
}

/* Formatter */

template <matrix::Numeric T>
struct std::formatter<Matrix<T>> {
  std::string element_spec{"{}"};

  constexpr auto parse(std::format_parse_context& ctx) {
      auto it{ctx.begin()};
      auto end{ctx.end()};
      if (it != end && *it != '}') {
        element_spec = "{:";
        while (it != end && *it != '}') {
          element_spec += *it++;
        }
        element_spec += "}";
      }
      return it;
  }

  auto format(const Matrix<T>& m, std::format_context& ctx) const {
    auto out{ctx.out()};
    const auto& data{m.get_data()};

    out  = std::format_to(out, "[\n");
    for (size_t i{0}; i < m.get_rows(); ++i) {
      out = std::format_to(out, " [");
      for (size_t j{0}; j < m.get_cols(); ++j) {
        out  = std::vformat_to(out, element_spec, std::make_format_args(data[i][j]));
        if (j + 1 < m.get_cols()) {
          out = std::format_to(out, ", ");
        }
      }
      out = std::format_to(out, "]{}\n", (i + 1 < m.get_rows() ? "," : ""));
    }
    return std::format_to(out, "]\n");
  }
};
