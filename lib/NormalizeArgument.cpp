#include "NormalizeArgument.h"

using namespace ArgumentParser;

NormalizedArgument SplitLongArgument(std::string arg_string) {
  NormalizedArgument arg;
  arg.long_name = arg_string.substr(2, arg_string.find('=') - 2);
  arg.type = ArgumentType::kLong;
  arg.value = arg_string.substr(arg_string.find('=') + 1);
  return arg;
}

NormalizedArgument SplitShortArgument(std::string arg_string) {
  NormalizedArgument arg;
  arg.short_name = arg_string[1];
  arg.type = ArgumentType::kShort;
  arg.value = arg_string.substr(arg_string.find('=') + 1);
  return arg;
}

std::vector<NormalizedArgument> SplitNestedShortArgument(std::string arg_string) {
  std::vector<NormalizedArgument> args;
  args.reserve(arg_string.size() - 1);
  for (std::size_t i = 1; i < arg_string.size(); ++i) {
    NormalizedArgument arg;
    arg.short_name = arg_string[i];
    arg.type = ArgumentType::kShort;
    arg.value = "";
    args.push_back(arg);
  }
  return args;
}

std::vector<NormalizedArgument> ArgumentParser::NormalizeArguments(std::vector<std::string> argv) {
  std::vector<NormalizedArgument> normalized;
  normalized.reserve(argv.size());
  for (std::size_t i = 1; i < argv.size(); ++i) {
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      if (argv[i].find('=') == std::string::npos) {
        NormalizedArgument arg;
        arg.long_name = argv[i].substr(2);
        arg.type = ArgumentType::kLong;
        if (argv.size() > i + 1 && argv[i + 1][0] != '-') {
          arg.value = argv[++i];
        } else {
          arg.value = "";
        }
        normalized.push_back(arg);
      } else {
        normalized.push_back(SplitLongArgument(argv[i]));
      }
    } else if (argv[i][0] == '-' && argv[i].size() > 2 && argv[i][2] != '=') {
      for (auto& arg : SplitNestedShortArgument(argv[i])) {
        normalized.push_back(arg);
      }
    } else if (argv[i][0] == '-') {
      if (argv[i].find('=') == std::string::npos) {
        NormalizedArgument arg;
        arg.long_name = argv[i].substr(1);
        arg.type = ArgumentType::kShort;
        if (argv.size() > i + 1 && argv[i + 1][0] != '-') {
          arg.value = argv[++i];
        } else {
          arg.value = "";
        }
        normalized.push_back(arg);
      } else {
        normalized.push_back(SplitShortArgument(argv[i]));
      }
    } else {
      NormalizedArgument arg;
      arg.type = ArgumentType::kPositional;
      arg.long_name = "";
      arg.value = argv[i];
      normalized.push_back(arg);
    }
  }
  return normalized;
}
