#include <cassert>

/* Constructors */

template <concepts::Numeric T>
Vector<T>::Vector(
  std::initializer_list<T> data)
  : _dimensions(data.size()),
    _data(data) {
}

template <concepts::Numeric T>
template <typename R>
requires concepts::flat_range<R, T>
Vector<T>::Vector(R&& data)
  : _dimensions(std::ranges::size(data)),
    _data(std::ranges::begin(data), std::ranges::end(data)) {
}

template <concepts::Numeric T>
template <concepts::Numeric U>
Vector<T>::Vector(
  const Vector<U>& other) 
  : _dimensions(other.get_dimensions()),
    _data(other.get_data()
      | std::views::transform([](const U& val) { return static_cast<T>(val); })
      | std::ranges::to<std::vector<T>>()) {
}

template <concepts::Numeric T>
size_t Vector<T>::get_dimensions() const {
  return _dimensions;
}

template <concepts::Numeric T>
const std::vector<T>& Vector<T>::get_data() const {
  return _data;
}

/* Membres - Operator overloads (Affectations) */

template <concepts::Numeric T> 
template <concepts::Numeric U>
Vector<T>& Vector<T>::operator+=(
  const Vector<U>& other) {
  
  assert(get_dimensions() == other.get_dimensions() && "Unable to add vectors with different sizes");
  const auto& data{other.get_data()};

  for (size_t i{0uz}; i < data.size(); ++i) {
    _data[i] += data[i];
  }

  return *this;
}

template <concepts::Numeric T>
template <concepts::Numeric U>
Vector<T>& Vector<T>::operator-=(
  const Vector<U>& other) {
  
  assert(get_dimensions() == other.get_dimensions() && "Unable to subtract vectors with different sizes");
  const auto& data{other.get_data()};

  for (size_t i{0uz}; i < data.size(); ++i) {
    _data[i] -= data[i];
  }

  return *this;
}

template <concepts::Numeric T>
template <concepts::Numeric U>
Vector<T>& Vector<T>::operator*=(
  const Vector<U>& other) {
  
  assert(get_dimensions() == other.get_dimensions() && "Unable to multiply vectors with different sizes");
  const auto& data{other.get_data()};

  for (size_t i{0uz}; i < data.size(); ++i) {
    _data[i] *= data[i];
  }

  return *this;
}

template <concepts::Numeric T>
template <concepts::Numeric U>
Vector<T>& Vector<T>::operator*=(const U& scalar) {
  for (size_t i{0uz}; i < _data.size(); ++i) {
    _data[i] *= scalar;
  }
  return *this;
}

/* Fonctions Libres - Operator overloads (Promotions) */

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator+(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<concepts::Promoted_Type<T, U>> {
  Vector<concepts::Promoted_Type<T, U>> result(lhs);
  result += rhs;
  return result;
}

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator-(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<concepts::Promoted_Type<T, U>> {
  Vector<concepts::Promoted_Type<T, U>> result(lhs);
  result -= rhs;
  return result;
}

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator*(const Vector<T>& lhs, const Vector<U>& rhs) -> Vector<concepts::Promoted_Type<T, U>> {
  Vector<concepts::Promoted_Type<T, U>> result(lhs);
  result *= rhs;
  return result;
}

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator*(const Vector<T>& lhs, const U& scalar) -> Vector<concepts::Promoted_Type<T, U>> {
  Vector<concepts::Promoted_Type<T, U>> result(lhs);
  result *= scalar;
  return result;
}

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto operator*(const T& scalar, const Vector<U>& rhs) -> Vector<concepts::Promoted_Type<T, U>> {
  return rhs * scalar;
}

template <concepts::Numeric T, concepts::Numeric U>
auto operator<=>(const Vector<T>& lhs, const Vector<U>& rhs) {
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
auto operator==(const Vector<T>& lhs, const Vector<U>& rhs) {
  const auto& lhs_data = lhs.get_data();
  const auto& rhs_data = rhs.get_data();

  return 
    lhs_data.size() == rhs_data.size() &&
    std::ranges::equal(lhs_data, rhs_data);
}

/* Norms */

// Manhattan norm
template <concepts::Numeric T>
[[nodiscard]] std::float32_t Vector<T>::norm_1() {
  auto norm = _data |
    std::views::transform(
      [](auto&& val){
        return std::abs(static_cast<std::float32_t>(val));
      }
    );

  return std::ranges::fold_left(norm, std::float32_t{0}, std::plus<>{});
}

// Euclidian norm
template <concepts::Numeric T>
[[nodiscard]] std::float32_t Vector<T>::norm() {

  auto norm = _data |
    std::views::transform(
      [](auto&& val){
        return static_cast<std::float32_t>(val * val);
      }
    );

  return 
    std::sqrt(
      std::ranges::fold_left(
        norm, 
        std::float32_t{0}, 
        std::plus<>{}
      )
    );
}

// Supremum norm
template <concepts::Numeric T>
[[nodiscard]] std::float32_t Vector<T>::norm_inf() {
  auto abs = _data |
    std::views::transform(
      [](auto&& val){
        return std::fabs(static_cast<std::float32_t>(val));
      }
    );

  return std::ranges::max(abs); 
}

/* Formatter */

template <concepts::Numeric T>
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
