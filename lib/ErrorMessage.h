#ifndef ARGUMENT_PARSER__LIB_ERROR_MESSAGE_H_
#define ARGUMENT_PARSER__LIB_ERROR_MESSAGE_H_

#include <iostream>

namespace ArgumentParser {
enum class ErrorMessages {
  kInvalidArgument,
  kTooManyArguments,
  kTooFewArguments,
  kMissingArgument,
};

void ErrorMessage(ErrorMessages error_message);

}// namespace ArgumentParser

#endif// ARGUMENT_PARSER__LIB_ERROR_MESSAGE_H_
