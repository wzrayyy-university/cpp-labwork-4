#ifndef ARGUMENT_PARSER_LIB_UTILS_H_
#define ARGUMENT_PARSER_LIB_UTILS_H_

#include <string>
#include <vector>

namespace ArgumentParser {

enum class ArgumentType {
  kShort,
  kLong,
  kPositional,
};

struct NormalizedArgument {
  ArgumentType type{};
  std::string long_name{};
  char short_name = '\0';
  std::string value{};
};

std::vector<NormalizedArgument> NormalizeArguments(std::vector<std::string> argv);

} // namespace ArgumentParser

#endif // ARGUMENT_PARSER_LIB_UTILS_H_