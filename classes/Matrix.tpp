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
//  _data.reserve(row * col);
//  _data.assign(std::ranges::begin(data), std::ranges::end(data));
  _data.assign_range(data);  
}

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>::Matrix(
  const Matrix<U>& other)
  : _rows(other.get_rows()),
    _cols(other.get_cols()),    
    _data(other.get_data() 
      | std::views::transform([](const U& val) { 
          return static_cast<T>(val); })
      | std::ranges::to<std::vector<T>>()) {
}

template <matrix::Numeric T>
size_t Matrix<T>::get_rows() const noexcept {
    return _rows;
}

template <matrix::Numeric T>
size_t Matrix<T>::get_cols() const noexcept {
    return _cols;
}

template<matrix::Numeric T>
std::pair<size_t, size_t> Matrix<T>::get_shape() const noexcept {
  return {_rows, _cols};
}

template <matrix::Numeric T>
//const std::vector<std::vector<T> >& Matrix<T>::get_data() const noexcept {
const std::vector<T>& Matrix<T>::get_data() const noexcept {
    return _data;
}

/* Operator overloads */

template <matrix::Numeric T>
template <matrix::Numeric U>
[[nodiscard]]
matrix::PMatrix<T, U> Matrix<T>::operator+(const Matrix<U>& other) const {
  if (get_shape() != other.get_shape()) {
    throw 
      std::invalid_argument(
        "Unable to add matrices with different shapes");
  }

  matrix::PMatrix<T, U> result(*this);
  result += other;
  return result;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>& Matrix<T>::operator+=(const Matrix<U>& other) {
  if (get_shape() != other.get_shape()) {
    throw 
      std::invalid_argument(
        "Unable to add matrices with different shapes");
  }

  const auto& rhs = other.get_data();
  const size_t size{_data.size()};
  for (size_t i{0uz}; i < size; ++i) {
    _data[i] = static_cast<T>(_data[i] + rhs[i]);
  }

  return *this;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
[[nodiscard]]
matrix::PMatrix<T, U> Matrix<T>::operator-(const Matrix<U>& other) const {
  if (get_shape() != other.get_shape()) {
    throw 
      std::invalid_argument(
        "Unable to subtract matrices with different shapes");
  }

  matrix::PMatrix<T, U> result(*this);
  result -= other;
  return result;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>& Matrix<T>::operator-=(const Matrix<U>& other) {
  if (get_shape() != other.get_shape()) {
    throw 
      std::invalid_argument(
        "Unable to subtract matrices with different shapes");
  }

  const auto& rhs = other.get_data();
  const size_t size{_data.size()};
  for (size_t i{0uz}; i < size; ++i) {
    _data[i] = static_cast<T>(_data[i] - rhs[i]);
  }

  return *this;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
[[nodiscard]]
matrix::PMatrix<T, U> Matrix<T>::operator*(
  const U& scalar) const {
  matrix::PMatrix<T, U> result(*this);
  result *= scalar;
  return result;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
Matrix<T>& Matrix<T>::operator*=(const U& scalar) {
  for (auto& value : _data) {
    value = static_cast<T>(value * scalar);
  }

  return *this;
}

template <matrix::Numeric T>
template <matrix::Numeric U>
[[nodiscard]]
matrix::PMatrix<T, U> Matrix<T>::operator*(
  const Matrix<U>& other) const {
  if (_cols != other.get_rows()) {
    throw 
      std::invalid_argument(
        "Unable to multiply matrices with incompatible shapes");
  }

  using R = matrix::promoted_type<T, U>;

  const size_t rows = _rows;
  const size_t inner = _cols;
  const size_t cols = other.get_cols();

  const auto& rhs = other.get_data();
  std::vector<R> result_data(rows * cols, R{0});

  for (size_t i = 0; i < rows; ++i) {
    const size_t i_inner_offset = i * inner;
    const size_t i_cols_offset = i * cols;
    
    for (size_t k = 0; k < inner; ++k) {
      const R lhs_val = static_cast<R>(_data[i_inner_offset + k]);
      const size_t k_cols_offset = k * cols;

      for (size_t j = 0; j < cols; ++j) {
        result_data[i_cols_offset + j] += 
          lhs_val * 
          static_cast<R>(rhs[k_cols_offset + j]);
      }
    }
  }

  // Si ton constructeur prend un std::vector par valeur et le move, c'est parfait
  return Matrix<R>(std::move(result_data), rows, cols);
}

//template <matrix::Numeric T>
//template <matrix::Numeric U>
//[[nodiscard]]
//matrix::PMatrix<T, U> Matrix<T>::operator*(
//  const Matrix<U>& other) const {
//
//  if (_cols != other.get_rows()) {
//    throw 
//      std::invalid_argument(
//        "Unable to multiply matrices with incompatible shapes");
//  }
//
//  using R = matrix::promoted_type<T, U>;
//
//  const size_t rows = _rows;
//  const size_t inner = _cols;
//  const size_t cols = other.get_cols();
//
//  std::vector<R> result_data(rows * cols, R{});
//
//  std::mdspan lhs(_data.data(), rows, inner);
//  std::mdspan rhs(other.get_data().data(), inner, cols);
//  std::mdspan res(result_data.data(), rows, cols);
//
//  for (size_t i = 0; i < rows; ++i) {
//    for (size_t k = 0; k < inner; ++k) {
//      const R lhs_val = static_cast<R>(lhs_view(i, k));    
//      for (size_t j = 0; j < cols; ++j) {
//        res_view(i, j) += lhs * static_cast<R>(rhs(k, j));
//      }
//    }
//  }
//
//  return Matrix<R>(std::move(result_data), rows, cols);
//}

template <matrix::Numeric T, matrix::Numeric U>
//[[nodiscard]]
auto operator<=>(const Matrix<T>& lhs, const Matrix<U>& rhs) {
  if (auto cmp = lhs.get_rows() <=> rhs.get_rows(); cmp != 0) return cmp;
  if (auto cmp = lhs.get_cols() <=> rhs.get_cols(); cmp != 0) return cmp;
  return lhs.get_data() <=> rhs.get_data();
}

template <matrix::Numeric T, matrix::Numeric U>
//[[nodiscard]]
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
    auto out = ctx.out();
    const size_t cols = m.get_cols();
    const size_t rows = m.get_rows();
    const auto& data = m.get_data();

    out = std::format_to(out, "[\n");
    for (size_t i = 0; i < rows; ++i) {
      out = std::format_to(out, " [");
      const size_t row_offset = i * cols; // Sorti de la boucle j
      for (size_t j = 0; j < cols; ++j) {
        out = std::vformat_to(
            out, 
            element_spec, 
            std::make_format_args(data[row_offset + j]));
        if (j + 1 < cols) {
          out = std::format_to(out, ", ");
        }
      }
      out = std::format_to(out, "]{}\n", (i + 1 < rows ? "," : ""));
    }
    return std::format_to(out, "]\n");
  }

//  auto format(const Matrix<T>& m, std::format_context& ctx) const {
//    auto out{ctx.out()};
//    size_t rows = m.get_rows();
//    size_t cols = m.get_cols();
//    
//    // Vue mdspan sur les données constantes de la matrice
//    std::mdspan view(m.get_data().data(), rows, cols);
//
//    out = std::format_to(out, "[\n");
//    for (size_t i = 0; i < rows; ++i) {
//      out = std::format_to(out, " [");
//      for (size_t j = 0; j < cols; ++j) {
//        out = std::vformat_to(
//          out, 
//          element_spec, 
//          std::make_format_args(view(i, j))
//        );
//        
//        if (j + 1 < cols) {
//          out = std::format_to(out, ", ");
//        }
//      }
//      out = std::format_to(out, "]{}\n", (i + 1 < rows ? "," : ""));
//    }
//    return std::format_to(out, "]\n");
//  }
};
