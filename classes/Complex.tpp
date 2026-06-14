template <complex::Numeric T>
Complex<T>::Complex(
  const T& real,
  const T& imag) 
  : _real(real),
  _imag(imag) {
}

template <complex::Numeric T>
template <complex::Numeric U>
Complex<T>::Complex(
  const Complex<U>& other) {
  using R = complex::promoted_type<T, U>;

  _real = static_cast<R>(other.get_real());
  _imag = static_cast<R>(other.get_imag());
}

template <complex::Numeric T>
T Complex<T>::get_real() const {
    return _real;
}

template <complex::Numeric T>
T Complex<T>::get_imag() const {
    return _imag;
}

template <complex::Numeric T>
struct std::formatter<Complex<T>> {
  std::string element_spec{"{}"};

  constexpr auto aprse(std::format_parse_context& ctx) {
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

    return std::format_to(out, "{} {}");
  }

};
