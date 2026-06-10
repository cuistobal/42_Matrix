template <typename T>
Matrix<T>::Matrix(Dataset data) {
  for (const auto& row: data) {
    _data.push_back(std::vector<T>(row));
  }
  _rows = _data.size();
  _cols = _data.empty() ? 0 : _data[0].size();
} 

template <typename T>
size_t Matrix<T>::get_rows() const {
    return _rows;
}

template <typename T>
size_t Matrix<T>::get_cols() const {
    return _cols;
}

template <typename T>
const std::vector<std::vector<T> >& Matrix<T>::get_data() const {
    return _data;
}

template <typename T>
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
    auto out{ctx.out()};
    const auto& data{m.get_data()};

    out  = std::format_to(out, "[\n");
    for (size_t i{0}; i < m.get_rows(); ++i) {
      out = std::format_to(out, " [");
      for (size_t j{0}; j < m.get_cols(); ++j) {
        out  = std::vformat_to(out, element_spec, std::make_format_args(data[i][j]));
        if (j + 1 < m.get_cols()) {
          out = std::format_to(out, ", ");
        }
      }
      out = std::format_to(out, "]{}\n", (i + 1 < m.get_rows() ? "," : ""));
    }
    return std::format_to(out, "]\n");
  }
};
