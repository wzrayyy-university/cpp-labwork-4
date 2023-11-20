#include "ParserArgument.h"

namespace ArgumentParser {

// --- default value ---
template<typename T>
ParserArgument<T>& ParserArgument<T>::Default(T value) {
  value_ = value;
  has_default_ = true;
  return *this;
};

// --- multi value ---
template<typename T>
ParserArgument<T>& ParserArgument<T>::MultiValue() {
  is_multi_value_ = true;
  return *this;
};

template<typename T>
ParserArgument<T>& ParserArgument<T>::MultiValue(std::size_t min_args_count) {
  is_multi_value_ = true;
  min_args_count_ = min_args_count;
  return *this;
};

// --- is_positional_ ---
template<typename T>
ParserArgument<T>& ParserArgument<T>::Positional() {
  is_positional_ = true;
  return *this;
};

// --- store value(s) ---
template<typename T>
ParserArgument<T>& ParserArgument<T>::StoreValue(T& container) {
  container_ = &container;
  return *this;
};

template<typename T>
ParserArgument<T>& ParserArgument<T>::StoreValues(std::vector<T>& container) {
  multi_container_ = &container;
  return *this;
};

template<typename T>
T ParserArgument<T>::GetValue() {
  if (container_ == nullptr) {
    return value_;
  } else {
    return *container_;
  }
}

template<typename T>
T ParserArgument<T>::GetMultiValue(std::size_t idx) {
  if (multi_container_ == nullptr) {
    return multi_value_[idx];
  } else {
    return (*multi_container_)[idx];
  }
};

template<typename T>
std::size_t ParserArgument<T>::GetMultiValueSize() {
  if (multi_container_ == nullptr) {
    return multi_value_.size();
  } else {
    return multi_container_->size();
  }
}

template<typename T>
void ParserArgument<T>::SetValue(T value) {
  has_value_ = true;
  if (is_multi_value_) {
    if (multi_container_ == nullptr) {
      multi_value_.push_back(value);
    } else {
      multi_container_->push_back(value);
    }
  } else {
    if (container_ == nullptr) {
      value_ = value;
    } else {
      *container_ = value;
    }
  }
}

}; //  namespace ArgumentParser

template class ArgumentParser::ParserArgument<std::string>;
template class ArgumentParser::ParserArgument<int>;
template class ArgumentParser::ParserArgument<bool>;