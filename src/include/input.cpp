#include <string>
#include <type_traits>

enum PN {
  Any,
  Positive_only,
  Negative_only,
};

template <typename T,
          std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
PN check_sign(T x) {
  if (x == 0)
    return PN::Any;
  if (x > 0)
    return PN::Positive_only;
  else
    return PN::Negative_only;
}

template <typename T, std::enable_if_t<std::is_integral<T>::value, bool> = true>
T input(const std::string &out, const std::string &err = "error") {
  return 0;
}

template <typename T,
          std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
T input(const std::string &out, const std::string &err = "error") {
  return 0.0;
}
