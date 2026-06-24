template <concepts::Numeric T, concepts::Numeric U>
auto vector_addition(const Vector<T>& lhs, const Vector<U>& rhs) {
	return lhs + rhs;
}

template <concepts::Numeric T, concepts::Numeric U>
auto vector_substraction(const Vector<T>& lhs, const Vector<U>& rhs) {
	return lhs - rhs;
}

template <concepts::Numeric T, concepts::Numeric U>
auto vector_scaling(const Vector<T>& lhs, const U& scalar) {
	return lhs * scalar;
}
