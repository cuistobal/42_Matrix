#include <cassert>

/* Ctors && Dtors */

template <concepts::Numeric T>
Matrix<T>::Matrix(Dataset data) 
  : Matrix<T>(std::views::all(data)) {
}

template <concepts::Numeric T>
template <typename R>
requires concepts::nested_range<R, T>
Matrix<T>::Matrix(R&& data) {

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
    assert(d == _cols && "Invalid matrix format: row size mismatch");
    
    _data.append_range(row);
  }
}

template <concepts::Numeric T>
template <typename R>
requires concepts::flat_range<R, T>
Matrix<T>::Matrix(
  R&& data,
  size_t row,
  size_t col) 
  : _rows(row),
  _cols(col) {
  size_t size{static_cast<size_t>(std::ranges::distance(data))};
  assert(size == row * col && "Data size doesn't match expected size");
  
  _data.assign_range(data);
}

template <concepts::Numeric T>
template <concepts::Numeric U>
Matrix<T>::Matrix(
  const Matrix<U>& other)
  : _rows(other.get_rows()),
    _cols(other.get_cols()),    
    _data(other.get_data() 
      | std::views::transform([](const U& val) { 
          return static_cast<T>(val); })
      | std::ranges::to<std::vector<T>>()) {
}

template <concepts::Numeric T>
size_t Matrix<T>::get_rows() const noexcept {
    return _rows;
}

template <concepts::Numeric T>
size_t Matrix<T>::get_cols() const noexcept {
    return _cols;
}

template<concepts::Numeric T>
std::pair<size_t, size_t> Matrix<T>::get_shape() const noexcept {
  return {_rows, _cols};
}

template <concepts::Numeric T>
const std::vector<T>& Matrix<T>::get_data() const noexcept {
    return _data;
}

/* Membres - Operator overloads (Affectations) */

template <concepts::Numeric T>
template <concepts::Numeric U>
Matrix<T>& Matrix<T>::operator+=(const Matrix<U>& other) {
  
  assert(get_shape() == other.get_shape() && "Unable to add matrices with different shapes");
  const auto& rhs = other.get_data();
  const size_t size{_data.size()};
  
  for (size_t i{0uz}; i < size; ++i) {
    _data[i] = static_cast<T>(_data[i] + rhs[i]);
  }

  return *this;
}

template <concepts::Numeric T>
template <concepts::Numeric U>
Matrix<T>& Matrix<T>::operator-=(const Matrix<U>& other) {
  
  assert(get_shape() == other.get_shape() && "Unable to subtract matrices with different shapes");
  const auto& rhs = other.get_data();
  const size_t size{_data.size()};
  
  for (size_t i{0uz}; i < size; ++i) {
    _data[i] = static_cast<T>(_data[i] - rhs[i]);
  }

  return *this;
}

template <concepts::Numeric T>
template <concepts::Numeric U>
Matrix<T>& Matrix<T>::operator*=(const U& scalar) {
  for (auto& value : _data) {
    value = static_cast<T>(value * scalar);
  }

  return *this;
}


/* Fonctions Libres - Operator overloads (Promotions) */

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator+(const Matrix<T>& lhs, const Matrix<U>& rhs) -> Matrix<concepts::Promoted_Type<T, U>> {
  Matrix<concepts::Promoted_Type<T, U>> result(lhs);
  result += rhs;
  return result;
}

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator-(const Matrix<T>& lhs, const Matrix<U>& rhs) -> Matrix<concepts::Promoted_Type<T, U>> {
  Matrix<concepts::Promoted_Type<T, U>> result(lhs);
  result -= rhs;
  return result;
}

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator*(const Matrix<T>& lhs, const Matrix<U>& rhs) -> Matrix<concepts::Promoted_Type<T, U>> {
  
  assert(lhs.get_cols() == rhs.get_rows() && "Unable to multiply matrices with incompatible shapes");
  using R = concepts::Promoted_Type<T, U>;

  const size_t rows = lhs.get_rows();
  const size_t inner = lhs.get_cols();
  const size_t cols = rhs.get_cols();
  const auto& lhs_data = lhs.get_data();
  const auto& rhs_data = rhs.get_data();
  
  std::vector<R> result_data(rows * cols, R{0});
  
  for (size_t i = 0; i < rows; ++i) {
    const size_t i_inner_offset = i * inner;
    const size_t i_cols_offset = i * cols;
    
    for (size_t k = 0; k < inner; ++k) {
      const R lhs_val = static_cast<R>(lhs_data[i_inner_offset + k]);
      const size_t k_cols_offset = k * cols;

      for (size_t j = 0; j < cols; ++j) {
        result_data[i_cols_offset + j] += 
          lhs_val * static_cast<R>(rhs_data[k_cols_offset + j]);
      }
    }
  }

  return Matrix<R>(std::move(result_data), rows, cols);
}

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator*(const Matrix<T>& lhs, const U& scalar) -> Matrix<concepts::Promoted_Type<T, U>> {
  Matrix<concepts::Promoted_Type<T, U>> result(lhs);
  result *= scalar;
  return result;
}

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator*(const T& scalar, const Matrix<U>& rhs) -> Matrix<concepts::Promoted_Type<T, U>> {
  return rhs * scalar;
}

template <concepts::Numeric T, concepts::Numeric U>
auto operator<=>(const Matrix<T>& lhs, const Matrix<U>& rhs) {
  if (auto cmp = lhs.get_rows() <=> rhs.get_rows(); cmp != 0) return cmp;
  if (auto cmp = lhs.get_cols() <=> rhs.get_cols(); cmp != 0) return cmp;
  return lhs.get_data() <=> rhs.get_data();
}

template <concepts::Numeric T, concepts::Numeric U>
auto operator==(const Matrix<T>& lhs, const Matrix<U>& rhs) {
  if constexpr (std::same_as<T, U>) {
    return lhs.get_rows() == rhs.get_rows() &&
      lhs.get_cols() == rhs.get_cols() &&
      lhs.get_data() == rhs.get_data();
  }
  return false;
}

/* Formatter */

template <concepts::Numeric T>
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

  auto format(const Matrix<T>& m, std::format_context& ctx) const {
    auto out = ctx.out();
    const size_t cols = m.get_cols();
    const size_t rows = m.get_rows();
    const auto& data = m.get_data();

    out = std::format_to(out, "[\n");
    for (size_t i = 0; i < rows; ++i) {
      out = std::format_to(out, " [");
      const size_t row_offset = i * cols;
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
};
