/* Ctors && Dtors */

template <matrix::Numeric T>
Matrix<T>::Matrix(Dataset data) 
  : Matrix<T>(std::views::all(data)) {
}

template <matrix::Numeric T>
template <typename R>
requires matrix::nested_range<R, T>
Matrix<T>::Matrix(R&& data) {

  // Quid de throw ?
  if (std::ranges::empty(data)) {
    _rows = 0;
    _cols = 0;
    return ;
  }

  _rows = static_cast<size_t>(std::ranges::distance(data));
  _cols = static_cast<size_t>(std::ranges::distance(*std::ranges::begin(data)));

  _data.reserve(_rows * _cols);

  for (const auto& row: data) {
    size_t d{static_cast<size_t>(std::ranges::distance(row))};
    if (d != _cols) {
      throw std::invalid_argument(
        std::format(
          "Invalid matrix format: expected {}, got {}",
          _cols, 
          d));
    }
    
    _data.append_range(row);
//    Less readbale equivalent
//    _data.insert(_data.end(), std::ranges::begin(row), std::ranges::end(row));
  }
}

template <matrix::Numeric T>
template <typename R>
requires matrix::flat_range<R, T>
Matrix<T>::Matrix(
  R&& data,
  size_t row,
  size_t col) 
  : _rows(row),
  _cols(col) {
  size_t size{static_cast<size_t>(std::ranges::distance(data))}; 
  if (size != row * col) {
    throw std::invalid_argument("Data size doesn't match expected size");
  }

  _data.assign(std::ranges::begin(data), std::ranges::end(data));
  
}

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>::Matrix(const Matrix<U>& other)
  : _rows(other.get_rows()),
  _cols(other.get_cols()),
//  _data(_rows, std::vector<T>(_cols)) {
  _data(_rows * _cols) {
  const auto& src = other.get_data();

  for (size_t i{0uz}; i < _rows; ++i) {
    for (size_t j{0uz}; j < _cols; ++j) {
      size_t idx{i * _cols + j};
      _data[idx] = static_cast<T>(src[idx]);
//      _data[i][j] = static_cast<T>(src[i][j]);
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
//const std::vector<std::vector<T> >& Matrix<T>::get_data() const {
const std::vector<T>& Matrix<T>::get_data() const {
    return _data;
}

/* Operator overloads */

template <matrix::Numeric T>
template <matrix::Numeric U>
matrix::PMatrix<T, U>
Matrix<T>::operator+(const Matrix<U>& other) const {
  matrix::PMatrix<T, U> tmp(*this);
  tmp += other;
  return tmp;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>& Matrix<T>::operator+=(const Matrix<U>& other) {
  const auto& data{other.get_data()};
  for (auto&& [lhs, rhs] : std::views::zip(_data, data)) {
    lhs += rhs;
  }
  return *this;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
matrix::PMatrix<T, U>
Matrix<T>::operator-(const Matrix<U>& other) const {
  matrix::PMatrix<T, U> tmp(*this);
  tmp -= other;
  return tmp;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>& Matrix<T>::operator-=(const Matrix<U>& other) {
  const auto& data{other.get_data()};
  for (auto&& [lhs, rhs] : std::views::zip(_data, data)) {
    lhs -= rhs;
  }
  return *this;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
matrix::PMatrix<T, U>
Matrix<T>::operator*(const Matrix<U>& other) const {
  matrix::PMatrix<T, U> tmp(*this);
  tmp *= other;
  return tmp;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>& Matrix<T>::operator*=(const Matrix<U>& other) {
  const auto& data{other.get_data()};
  for (auto&& [lhs, rhs] : std::views::zip(_data, data)) {
      lhs *= rhs;
  }
  return *this;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
matrix::PMatrix<T, U>
Matrix<T>::operator*(const U& scalar) const {
  matrix::PMatrix<T, U> tmp(*this);
  tmp *= scalar;
  return tmp;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>& Matrix<T>::operator*=(const U& scalar) {
  for (auto& val: _data) {
    val *= scalar;
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
    size_t cols{m.get_cols()};
    const auto& data{m.get_data()};

    out  = std::format_to(out, "[\n");
    for (size_t i{0}; i < m.get_rows(); ++i) {
      out = std::format_to(out, " [");
      for (size_t j{0}; j < m.get_cols(); ++j) {
        out = 
          std::vformat_to(
            out, 
            element_spec, 
            std::make_format_args(data[i * cols + j]));
        if (j + 1 < m.get_cols()) {
          out = std::format_to(out, ", ");
        }
      }
      out = std::format_to(out, "]{}\n", (i + 1 < m.get_rows() ? "," : ""));
    }
    return std::format_to(out, "]\n");
  }
};
