//template <vector::Numeric T>
//Vector<T>::Vector(std::initializer_list<T> data) 
//  : _dimensions(data.size()),
//  _data(data) {
//}
//
//size_t Vector<T>::get_dimensions() const {
//  return _dimensions;
//}
//
//const std::vector<T>& Vector<T>::get_data() const {
//  return _data;
//}
//
//template <vector::Numeric T>
//struct std::formatter<Vector<T>> {
//
//  std::string element_spec{"{}"};
//
//  constexpr auto parse(std::format_parse_context& ctx) {
//    auto it{ctx.begin()};
//    auto end{ctx.end()};
//
//    if (it != end && *it != '}') {
//      element_spec = "{:";
//      while (it != end && *it != '}') {
//        element_spec += *it++; 
//      }
//      element_spec += "}";
//    }
//    return it;
//  }
//
//  auto format(const Vector<T>& v, std::format_context& ctx) const {
//    auto out{ctx.out};
//    const auto& data{v.get_data()};
//
//    out  = std::format_to(out, "[ ");
//    for (size_t i{0}; i < data.size(); ++i) {
//        out = std::vformat_to(out, element_spec, std::make_format_args(data[i]));
//        if (i + 1 < data.size()) {
//          out += std::format_to(out, ", ");
//        }
//      }
//    }
//    return std::format_to(out, " ]\n");
//  }
//};

template <vector::Numeric T>
Vector<T>::Vector(std::initializer_list<T> data)
    : _dimensions(data.size()),
      _data(data)
{
}

template <vector::Numeric T>
size_t Vector<T>::get_dimensions() const {
  return _dimensions;
}

template <vector::Numeric T>
const std::vector<T>& Vector<T>::get_data() const {
  return _data;
}

template <vector::Numeric T>
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
