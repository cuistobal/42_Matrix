template <complex::Numeric T>
Complex<T>::Complex(const T& real, const T& imag) 
  : _real(real),
  _imag(image) {
}

template <complex::Numeric T>
template <complex::Numeric U>
Complex<T>::Complex(
  const Complex<U>& other)
  : _real(real),
  _imag(image) {
}

template <complex::Numeric T>
Complex<T>::Complex(
  const Compelx<T>& other)
  : _real(real),
  _imag(image) {
}
