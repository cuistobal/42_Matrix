template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto add(const Vector<T>& lhs, const Vector<U>& rhs) noexcept {
	return lhs + rhs;
}

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto sub(const Vector<T>& lhs, const Vector<U>& rhs) noexcept {
	return lhs - rhs;
}

template <concepts::Numeric T, concepts::Numeric U>
[[nodiscard]] auto scl(const Vector<T>& lhs, const U& scalar) noexcept {
	return lhs * scalar;
}
