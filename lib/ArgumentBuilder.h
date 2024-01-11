#pragma once

#include <string>
#include <vector>

#include "ParserArgument.h"

namespace ArgumentParser {

class IArgumentBuilder {
 public:
  // virtual ~IArgumentBuilder() = default;
  virtual IParserArgument* Build() = 0;

  bool has_default_ = false;
  bool has_value_ = false;
  std::size_t min_args_count_ = 0;
  bool is_multi_value_ = false;
  bool is_positional_ = false;
  bool is_help_ = false;

  std::string long_name{};
  char short_name = '\0';
  std::string description{};
};

template<typename T>
class ArgumentBuilder : public IArgumentBuilder {
 private:
  T value_ = T();
  T* container_ = nullptr;
  std::vector<T>* multi_container_ = nullptr;

 public:
  ArgumentBuilder() = default;

  ArgumentBuilder<T>& Default(T value);

  ArgumentBuilder<T>& MultiValue();

  ArgumentBuilder<T>& MultiValue(std::size_t min_args_count);

  ArgumentBuilder<T>& Positional();

  void SetValue(T value);

  ArgumentBuilder<T>& StoreValue(T& container);

  ArgumentBuilder<T>& StoreValues(std::vector<T>& container);

  IParserArgument* Build();
};

} // namespace ArgumentParser
