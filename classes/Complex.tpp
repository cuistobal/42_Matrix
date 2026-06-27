template <concepts::Numeric T>
Complex<T>::Complex(
  const T& real,
  const T& imag) 
  : _real(real),
  _imag(imag) {
}

template <concepts::Numeric T>
template <concepts::Numeric U>
Complex<T>::Complex(
  const Complex<U>& other) :
  _real(static_cast<T>(other.get_real())),
  _imag(static_cast<T>(other.get_imag())) {
}

template <concepts::Numeric T>
T Complex<T>::get_real() const {
  return _real;
}

template <concepts::Numeric T>
T Complex<T>::get_imag() const {
  return _imag;
}

template <concepts::Numeric T>
struct std::formatter<Complex<T>> {
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

  auto format(const Complex<T>& c, std::format_context& ctx) const {
    auto out{ctx.out()};
    T real{c.get_real()};
    T imag{c.get_imag()};

    return std::format_to(out, "{} {}", real, imag);
  }

};

template <concepts::Numeric T, concepts::Numeric U>
auto operator<=>(const Complex<T>& lhs, const Complex<U>& rhs) {
  using Ordering = std::partial_ordering;

  auto lhs_real = lhs.get_real();
  auto rhs_real = rhs.get_real();

  if (lhs_real != rhs_real) {
    return lhs_real < rhs_real ? Ordering::less : Ordering::greater;
  }

  auto lhs_imag = lhs.get_imag();
  auto rhs_imag = rhs.get_imag();

  if (lhs_imag != rhs_imag) {
    return lhs_imag < rhs_imag ? Ordering::less : Ordering::greater;
  }

  return Ordering::equivalent;
}

template <concepts::Numeric T, concepts::Numeric U>
auto operator==(const Complex<T>& lhs, const Complex<U>& rhs) {
  return lhs.get_real() == rhs.get_real() && lhs.get_imag() == rhs.get_imag();
}
