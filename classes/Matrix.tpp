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
[[nodiscard]] size_t Matrix<T>::get_rows() const noexcept {
    return _rows;
}

template <concepts::Numeric T>
[[nodiscard]] size_t Matrix<T>::get_cols() const noexcept {
    return _cols;
}

template<concepts::Numeric T>
[[nodiscard]] std::pair<size_t, size_t> Matrix<T>::get_shape() const noexcept {
  return {_rows, _cols};
}

template <concepts::Numeric T>
[[nodiscard]] const std::vector<T>& Matrix<T>::get_data() const noexcept {
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


/* Norms */

// Frobenius
template <concepts::Numeric T>
[[nodiscard]] std::float32_t Matrix<T>::norm() const noexcept {
  std::float32_t n{0.0f};

  auto abs = _data |
    std::views::transform(
      [](auto x){ 
        return std::abs(x);
      }
    );

  auto max = *std::ranges::max_element(abs);

  auto norm = _data |
    std::views::transform(
      [max](T elem){
        T scaled = elem / max;
        return scaled * scaled; 
      }
    );

  auto sum = 
    std::ranges::fold_left(
     norm, 
     0.0, 
     std::plus<T>());

  return static_cast<std::float32_t>(max * std::sqrt(sum));
}

template <concepts::Numeric T>
[[nodiscard]] std::float32_t Matrix<T>::norm_1() const noexcept {

  std::vector<T> norms(_cols, 0);

  for (size_t crow{0uz}; crow < _rows; ++crow) {
    size_t offset{crow * _cols};
    for (size_t ccol{0uz}; ccol < _cols; ++ccol) {
      norms[ccol] += std::abs(_data[offset + ccol]);
    }
  }

  return static_cast<std::float32_t>(*std::ranges::max_element(norms));
}

template <concepts::Numeric T>
[[nodiscard]] std::float32_t Matrix<T>::norm_inf() const noexcept {

  auto sum = _data |
    std::views::chunk(_cols) |
    std::views::transform(
      [](auto row){
        return std::ranges::fold_left(row, 0.0, 
          [](double sum, double elem){
            return sum + elem;
        }
      );
    }
  );

  return static_cast<std::float32_t>(std::ranges::max_element(sum));
}

/* Row echelon form */

//template <concepts::Numeric T> 
//[[nodiscard]] Matrix<T> Matrix<T>::row_echelon_form() const noexcept {
//
//  std::vector<T> data{_data};
//
//  for (size_t r{0uz}; r < (_rows - 1); ++r) {
//
//    // Find a valid pivot
//    size_t pindex{r * _cols + r}, roff{pindex};
//    double pivot{static_cast<double>(data[pindex]});
//    for (size_t i{1uz}; i < _rows; ++i) {
//      roff += _cols;
//      if (data[np]) {
//        pivot = static_cast<double>(data[np]);
//        std::swap_ranges(data[pindex], data[pindex + _cols], data[np]);
//      }
//    }
//
//    // Restablish the original offset value;
//    roff = pindex;
//
//    // Reduce subsequent rows
//    for (size_t i{r + 1}; i < _rows; ++i) {
//
//      roff += _cols;
//      double coef{static_cast<double>(data[roff] / pivot};
//
//      auto sit = data.begin() + roff;
//      auto eit = sit + _cols;
//
//      std::transform(sit, eit, sit, 
//        [](T val){
//          return static_cast<double>(val) - (coef;
//          }); 
//
//  }
//
//
//  return Matrix<T> {std::move(data), _rows, _cols};
//}

template <concepts::Numeric T> 
[[nodiscard]] Matrix<T> Matrix<T>::row_echelon_form() const noexcept {
    // On copie les données pour travailler en place
    std::vector<T> data = _data; 

    for (size_t r = 0; r < (_rows - 1); ++r) {
        size_t pivot_row = r;
        
        // 1. Recherche du pivot (sécurisée)
        while (pivot_row < _rows && data[pivot_row * _cols + r] == T{}) {
            ++pivot_row;
        }

        // Si aucun pivot non nul n'est trouvé, on passe à la colonne suivante
        if (pivot_row == _rows) continue; 

        // Si le pivot est sur une autre ligne, on échange les lignes
        if (pivot_row != r) {
            auto src_it = data.begin() + r * _cols;
            auto dst_it = data.begin() + pivot_row * _cols;
            std::swap_ranges(src_it, src_it + _cols, dst_it);
        }

        T pivot = data[r * _cols + r];

        // 2. Réduction des lignes suivantes
        for (size_t i = r + 1; i < _rows; ++i) {
            size_t target_row_start = i * _cols;
            size_t pivot_row_start = r * _cols;
            
            T factor = data[target_row_start + r] / pivot;

            // On soustrait (factor * ligne_pivot) à la ligne actuelle
            for (size_t c = r; c < _cols; ++c) {
                data[target_row_start + c] -= factor * data[pivot_row_start + c];
            }
        }
    }

    // Hypothèse : ton constructeur de Matrix accepte le vecteur et les dimensions
    return Matrix<T>(std::move(data), _rows, _cols);
}

/* Determinant */

template <concepts::Numeric T>
[[nodiscard]] std::float32_t Matrix<T>::determinant() const noexcept {
  std::float32_t det{0.0f};

  // Start by getting an upper triangular matrix
  Matrix<T> ref{row_echelon_form};

  // Return the sum of it's diagonaled values

//  auto ret = ref |
//  std::views::chunk(_rows) |
//  std::views::fold_left(
//    [](auto row){
//      return row[c];
//    });

  return det;
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
  auto cmp_dims = lhs.get_dimensions() <=> rhs.get_dimensions(); 

  if (cmp_dims != 0) {
    return cmp_dims;
  }
  
  const auto& lhs_data = lhs.get_data();
  const auto& rhs_data = rhs.get_data();

  return std::lexicographical_compare_three_way(
    lhs_data.begin(), lhs_data.end(),
    rhs_data.begin(), rhs_data.end()
  );
}

template <concepts::Numeric T, concepts::Numeric U>
auto operator==(const Matrix<T>& lhs, const Matrix<U>& rhs) {
  return lhs.get_rows() == rhs.get_rows() &&
    lhs.get_cols() == rhs.get_cols() &&
    std::ranges::equal(lhs.get_data(), rhs.get_data());
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
