#pragma once

#include <string>
#include <vector>

namespace ArgumentParser {

class IParserArgument {
 public:
  bool has_default_ = false;
  bool has_value_ = false;
  std::size_t min_args_count_ = 0;
  bool is_multi_value_ = false;
  bool is_positional_ = false;
  bool is_help_ = false;

  std::string long_name{};
  char short_name = '\0';
  std::string description{};

 public:
  virtual std::size_t GetMultiValueSize() = 0;
};

template<typename T>
class ParserArgument : public IParserArgument {
 private:
  T value_;
  T* container_;
  std::vector<T> multi_value_{};
  std::vector<T>* multi_container_;

 public:
  ParserArgument(bool has_default, bool has_value, std::size_t min_args_count, bool is_positional, bool is_help, T* container,
                 std::vector<T>* multi_container, T value, std::string long_name, char short_name, std::string description,
                 bool is_multi_value)
      : container_(container), multi_container_(multi_container), value_(value) {
    this->has_default_ = has_default;
    this->has_value_ = has_value;
    this->min_args_count_ = min_args_count;
    this->is_positional_ = is_positional;
    this->is_help_ = is_help;
    this->long_name = long_name;
    this->short_name = short_name;
    this->description = description;
    this->is_multi_value_ = is_multi_value;
  };
  ~ParserArgument() = default;
  T GetValue();
  T GetMultiValue(std::size_t idx);
  std::size_t GetMultiValueSize();
  void SetValue(T value);
};

} // namespace ArgumentParser
