#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "ArgumentBuilder.h"
#include "Utils.h"

namespace ArgumentParser {

class ArgParser {
 private:
  std::string name_;
  std::string description_;
  bool has_help_ = false;

  // template<typename T>
  // struct Arguments {
  //   std::map<T, ArgumentBuilder<std::string>*> string_args;
  //   std::map<T, ArgumentBuilder<int>*> int_args;
  //   std::map<T, ArgumentBuilder<bool>*> flag_args;
  // };

  std::vector<IArgumentBuilder*> argument_builders_;

  std::map<std::string, IParserArgument*> arguments_;
  std::map<char, std::string> short_to_long_;

  // Arguments<char> short_arguments_;
  // Arguments<std::string> long_arguments_;

  enum class ValueTypes {
    kString,
    kInt,
    kFlag,
  };

  std::map<std::string, ValueTypes> long_value_types_;
  std::map<char, ValueTypes> short_value_types_;

 private:
  template<typename T>
  ArgumentBuilder<T>* CreateArgument();

  bool CheckForInvalidArgs();

  std::string FindPositionalArgumentName();

 public:
  ArgParser(const std::string& name);

  // ~ArgParser();

  bool Parse(const std::vector<std::string>& argv);

  bool Parse(int argc, char** argv);

  void BuildArguments();

  // --- string argument ---
  ArgumentBuilder<std::string>& AddStringArgument(const std::string& long_name);

  ArgumentBuilder<std::string>& AddStringArgument(char short_name, const std::string& long_name);

  ArgumentBuilder<std::string>& AddStringArgument(const std::string& long_name, const std::string& description);

  ArgumentBuilder<std::string>& AddStringArgument(char short_name, const std::string& long_name, const std::string& description);

  std::string GetStringValue(const std::string& long_name);

  std::string GetStringValue(const std::string& short_name, std::size_t idx);

  // --- int argument ---
  ArgumentBuilder<int>& AddIntArgument(const std::string& long_name);

  ArgumentBuilder<int>& AddIntArgument(char short_name, const std::string& long_name);

  ArgumentBuilder<int>& AddIntArgument(const std::string& long_name, const std::string& description);

  ArgumentBuilder<int>& AddIntArgument(char short_name, const std::string& long_name, const std::string& description);

  int GetIntValue(const std::string& arg);

  int GetIntValue(const std::string& arg, std::size_t idx);

  // --- flag argument ---
  ArgumentBuilder<bool>& AddFlag(const std::string& long_name);

  ArgumentBuilder<bool>& AddFlag(char short_name, const std::string& long_name);

  ArgumentBuilder<bool>& AddFlag(const std::string& long_name, const std::string& description);

  ArgumentBuilder<bool>& AddFlag(char short_name, const std::string& long_name, const std::string& description);

  bool GetFlag(const std::string& arg);

  // --- help (me) ---
  ArgumentBuilder<bool>& AddHelp(char short_arg, const std::string& arg, const std::string& description);

  bool Help();

  std::string HelpDescription();
};

} // namespace ArgumentParser
