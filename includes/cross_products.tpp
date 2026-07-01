template<concepts::Numeric T>
Vector<T> cross_product(const Vector<T>& lhs, const Vector<T>& rhs) {

  std::vector<T> data;
  const auto& lhsd{lhs.get_data()};
  const auto& rhsd{rhs.get_data()};

  data.push_back((lhsd[1] * rhsd[2]) - (lhsd[2] * rhsd[1]));
  data.push_back((lhsd[2] * rhsd[0]) - (lhsd[0] * rhsd[2]));
  data.push_back((lhsd[0] * rhsd[1]) - (lhsd[1] * rhsd[0]));

  return Vector<T>{std::move(data)};
}
