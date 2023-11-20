#include "ErrorMessage.h"

using namespace ArgumentParser;

void ErrorMessage(ErrorMessages error_message) {
  switch (error_message) {
    case ErrorMessages::kInvalidArgument:
      std::cerr << "[ERROR] Invalid argument!";
      break;
    case ErrorMessages::kMissingArgument:
      std::cerr << "[ERROR] Missing argument!";
      break;
    case ErrorMessages::kTooFewArguments:
      std::cerr << "[ERROR] Too few arguments!";
      break;
    case ErrorMessages::kTooManyArguments:
      std::cerr << "[ERROR] Too many arguments";
      break;
    default:
      std::cerr << "[ERROR] Unknown error!";
      break;
  }
}