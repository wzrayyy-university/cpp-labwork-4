#ifndef ARGUMENT_PARSER_LIB_ARGPARSER_H_
#define ARGUMENT_PARSER_LIB_ARGPARSER_H_

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "ErrorMessage.h"
#include "ParserArgument.h"
#include "Utils.h"

namespace ArgumentParser {

class ArgParser {
 private:
  std::string name_;
  std::string description_;
  bool has_help_ = false;

  template<typename T>
  struct Arguments {
    std::map<T, ParserArgument<std::string>*> string_args;
    std::map<T, ParserArgument<int>*> int_args;
    std::map<T, ParserArgument<bool>*> flag_args;
  };

  Arguments<char> short_arguments_;
  Arguments<std::string> long_arguments_;

  enum class ValueTypes {
    kString,
    kInt,
    kFlag,
  };

  std::map<std::string, ValueTypes> long_value_types_;
  std::map<char, ValueTypes> short_value_types_;

 private:
  template<typename T>
  ParserArgument<T>* CreateArgument();

  bool CheckForInvalidArgs();

  std::string FindPositionalArgumentName();

 public:
  ArgParser(const std::string& name);

  // ~ArgParser();

  bool Parse(const std::vector<std::string>& argv);

  bool Parse(int argc, char** argv);

  // --- string argument ---
  ParserArgument<std::string>& AddStringArgument(const std::string& long_name);

  ParserArgument<std::string>& AddStringArgument(char short_name, const std::string& long_name);

  ParserArgument<std::string>& AddStringArgument(const std::string& long_name, const std::string& description);

  ParserArgument<std::string>& AddStringArgument(char short_name, const std::string& long_name, const std::string& description);

  std::string GetStringValue(const std::string& long_name);

  std::string GetStringValue(const std::string& short_name, std::size_t idx);

  // --- int argument ---
  ParserArgument<int>& AddIntArgument(const std::string& long_name);

  ParserArgument<int>& AddIntArgument(char short_name, const std::string& long_name);

  ParserArgument<int>& AddIntArgument(const std::string& long_name, const std::string& description);

  ParserArgument<int>& AddIntArgument(char short_name, const std::string& long_name, const std::string& description);

  int GetIntValue(const std::string& arg);

  int GetIntValue(const std::string& arg, std::size_t idx);

  // --- flag argument ---
  ParserArgument<bool>& AddFlag(const std::string& long_name);

  ParserArgument<bool>& AddFlag(char short_name, const std::string& long_name);

  ParserArgument<bool>& AddFlag(const std::string& long_name, const std::string& description);

  ParserArgument<bool>& AddFlag(char short_name, const std::string& long_name, const std::string& description);

  bool GetFlag(const std::string& arg);

  // --- help (me) ---
  ParserArgument<bool>& AddHelp(char short_arg, const std::string& arg, const std::string& description);

  bool Help();

  std::string HelpDescription();
};

} // namespace ArgumentParser

#endif // ARGUMENT_PARSER_LIB_ARGPARSER_H_