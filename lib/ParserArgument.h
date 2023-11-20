#ifndef ARGUMENT_PARSER_PARSER_ARGUMENT_H_
#define ARGUMENT_PARSER_PARSER_ARGUMENT_H_

#include <string>
#include <vector>

namespace ArgumentParser {

template<typename T>
class ParserArgument {
 private:
  T value_{};
  std::vector<T> multi_value_{};
  T* container_ = nullptr;
  std::vector<T>* multi_container_{};
  bool is_multi_value_ = false;

 public:
  std::string long_name{};
  char short_name = '\0';
  std::string description{};

  bool has_default_ = false;
  bool has_value_ = false;
  std::size_t min_args_count_ = 0;
  bool is_positional_ = false;
  bool is_help_ = false;

 public:
  ParserArgument() = default;

  ParserArgument& Default(T value);

  T GetMultiValue(std::size_t idx);

  std::size_t GetMultiValueSize();

  T GetValue();

  ParserArgument& MultiValue();

  ParserArgument& MultiValue(std::size_t min_args_count);

  ParserArgument& Positional();

  void SetValue(T value);

  ParserArgument& StoreValue(T& container);

  ParserArgument& StoreValues(std::vector<T>& container);
};

} // namespace ArgumentParser
#endif // ARGUMENT_PARSER_PARSER_ARGUMENT_H_