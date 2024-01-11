#include "ParserArgument.h"

using namespace ArgumentParser;

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
  if (this->multi_container_ == nullptr) {
    return this->multi_value_.size();
  } else {
    return this->multi_container_->size();
  }
}

template<typename T>
void ParserArgument<T>::SetValue(T value) {
  has_value_ = true;
  if (this->is_multi_value_) {
    if (this->multi_container_ == nullptr) {
      this->multi_value_.push_back(value);
    } else {
      this->multi_container_->push_back(value);
    }
  } else {
    if (this->container_ == nullptr) {
      this->value_ = value;
    } else {
      *this->container_ = value;
    }
  }
}

template class ArgumentParser::ParserArgument<std::string>;
template class ArgumentParser::ParserArgument<int>;
template class ArgumentParser::ParserArgument<bool>;
