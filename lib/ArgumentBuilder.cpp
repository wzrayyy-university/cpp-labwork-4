#include "ArgumentBuilder.h"

namespace ArgumentParser {

// --- default value ---
template<typename T>
ArgumentBuilder<T>& ArgumentBuilder<T>::Default(T value) {
  this->value_ = value;
  has_default_ = true;
  return *this;
}

// --- multi value ---
template<typename T>
ArgumentBuilder<T>& ArgumentBuilder<T>::MultiValue() {
  this->is_multi_value_ = true;
  return *this;
}

template<typename T>
ArgumentBuilder<T>& ArgumentBuilder<T>::MultiValue(std::size_t min_args_count) {
  this->is_multi_value_ = true;
  min_args_count_ = min_args_count;
  return *this;
}

// --- is_positional_ ---
template<typename T>
ArgumentBuilder<T>& ArgumentBuilder<T>::Positional() {
  is_positional_ = true;
  return *this;
}

// --- store value(s) ---
template<typename T>
ArgumentBuilder<T>& ArgumentBuilder<T>::StoreValue(T& container) {
  this->container_ = &container;
  return *this;
}

template<typename T>
ArgumentBuilder<T>& ArgumentBuilder<T>::StoreValues(std::vector<T>& container) {
  this->multi_container_ = &container;
  return *this;
}

template<typename T>
IParserArgument* ArgumentBuilder<T>::Build() {
  return new ParserArgument<T>(this->has_default_, this->has_value_, this->min_args_count_, this->is_positional_, this->is_help_,
                               this->container_, this->multi_container_, this->value_, this->long_name, this->short_name, this->description,
                               this->is_multi_value_);
}

}; //  namespace ArgumentParser

template class ArgumentParser::ArgumentBuilder<std::string>;
template class ArgumentParser::ArgumentBuilder<int>;
template class ArgumentParser::ArgumentBuilder<bool>;
