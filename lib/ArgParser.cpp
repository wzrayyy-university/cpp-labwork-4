#include "ArgParser.h"

using namespace ArgumentParser;

void ArgParser::BuildArguments() {
  for (IArgumentBuilder* argument : argument_builders_) {
    arguments_[argument->long_name] = argument->Build();
    if (argument->short_name != '\0') {
      short_to_long_[argument->short_name] = argument->long_name;
    }
  }
}

// --- ArgParser ---
ArgParser::ArgParser(const std::string& name) : name_(name){};

// --- Parse ---
bool ArgParser::Parse(const std::vector<std::string>& argv) {
  BuildArguments();
  std::vector<NormalizedArgument> normalized = NormalizeArguments(argv);

  for (std::size_t i = 0; i < normalized.size(); ++i) {
    if (normalized[i].type == ArgumentType::kShort) {
      normalized[i].long_name = short_to_long_[normalized[i].short_name];
    }
    if (normalized[i].type == ArgumentType::kLong || normalized[i].type == ArgumentType::kShort) {

      if (long_value_types_[normalized[i].long_name] == ValueTypes::kString) {
        reinterpret_cast<ParserArgument<std::string>*>(arguments_[normalized[i].long_name])->SetValue(normalized[i].value);
      } else if (long_value_types_[normalized[i].long_name] == ValueTypes::kInt) {
        reinterpret_cast<ParserArgument<int>*>(arguments_[normalized[i].long_name])->SetValue(std::stoi(normalized[i].value));
      } else if (long_value_types_[normalized[i].long_name] == ValueTypes::kFlag) {
        if (arguments_[normalized[i].long_name]->is_help_) { // ugh
          has_help_ = true;
          return true;
        }
        reinterpret_cast<ParserArgument<bool>*>(arguments_[normalized[i].long_name])->SetValue(true);
      }
    } else if (normalized[i].type == ArgumentType::kPositional) {
      std::string name = FindPositionalArgumentName();
      if (long_value_types_[name] == ValueTypes::kString) {
        reinterpret_cast<ParserArgument<std::string>*>(arguments_[name])->SetValue(normalized[i].value);
      } else if (long_value_types_[name] == ValueTypes::kInt) {
        reinterpret_cast<ParserArgument<int>*>(arguments_[name])->SetValue(std::stoi(normalized[i].value));
      } else if (long_value_types_[name] == ValueTypes::kFlag) {
        reinterpret_cast<ParserArgument<bool>*>(arguments_[name])->SetValue(true);
      }
    }
  }
  return CheckForInvalidArgs();
  // return true;
};

bool ArgParser::Parse(int argc, char** argv) {
  return Parse(std::vector<std::string>(argv, argv + argc));
}

// // -- Has invalid arguments --
bool ArgParser::CheckForInvalidArgs() {
  for (auto pair : arguments_) {
    if ((!pair.second->has_default_ && !pair.second->has_value_) && (!pair.second->is_help_ || !pair.second->is_positional_)) {
      return false;
    }
    if (pair.second->GetMultiValueSize() < pair.second->min_args_count_) {
      return false;
    }
  }
  return true;
}

// bool ArgParser::CheckForInvalidArgs() {
//   return CheckForInvalidArgs_<std::string>(long_arguments_.string_args) && CheckForInvalidArgs_<int>(long_arguments_.int_args)
//       && CheckForInvalidArgs_<bool>(long_arguments_.flag_args);
// }

// --- Find positional argument name ---
std::string ArgParser::FindPositionalArgumentName() {
  for (auto pair : arguments_) {
    if (pair.second->is_positional_) {
      return pair.first;
    }
  }

  return "";
}

// --- Create argument ---
template<typename T>
ArgumentBuilder<T>* ArgParser::CreateArgument() {
  return new ArgumentBuilder<T>;
}

// --- String argument ---
ArgumentBuilder<std::string>& ArgParser::AddStringArgument(const std::string& long_name) {
  auto* argument = CreateArgument<std::string>();
  argument->long_name = long_name;
  long_value_types_[long_name] = ValueTypes::kString;
  argument_builders_.push_back(argument);
  return *argument;
}

ArgumentBuilder<std::string>& ArgParser::AddStringArgument(const char short_name, const std::string& long_name) {
  auto* argument = &AddStringArgument(long_name);
  short_value_types_[short_name] = ValueTypes::kString;
  argument->short_name = short_name;
  argument_builders_.push_back(argument);
  return *argument;
}

ArgumentBuilder<std::string>& ArgParser::AddStringArgument(const std::string& long_name, const std::string& description) {
  auto* argument = &AddStringArgument(long_name);
  argument->description = description;
  return *argument;
}

ArgumentBuilder<std::string>& ArgParser::AddStringArgument(const char short_name, const std::string& long_name,
                                                           const std::string& description) {
  auto* argument = &AddStringArgument(short_name, long_name);
  argument->description = description;
  return *argument;
}

std::string ArgParser::GetStringValue(const std::string& long_name) {
  return reinterpret_cast<ParserArgument<std::string>*>(arguments_[long_name])->GetValue();
}

std::string ArgParser::GetStringValue(const std::string& long_name, const std::size_t idx) {
  return reinterpret_cast<ParserArgument<std::string>*>(arguments_[long_name])->GetMultiValue(idx);
}

// --- Int argument ---
ArgumentBuilder<int>& ArgParser::AddIntArgument(const std::string& long_name) {
  auto* argument = CreateArgument<int>();
  argument->long_name = long_name;
  long_value_types_[long_name] = ValueTypes::kInt;
  argument_builders_.push_back(argument);
  return *argument;
}

ArgumentBuilder<int>& ArgParser::AddIntArgument(char short_name, const std::string& long_name) {
  auto& argument = AddIntArgument(long_name);
  argument.short_name = short_name;
  short_value_types_[short_name] = ValueTypes::kInt;
  argument_builders_.push_back(&argument);
  return argument;
}

ArgumentBuilder<int>& ArgParser::AddIntArgument(const std::string& long_name, const std::string& description) {
  auto& argument = AddIntArgument(long_name);
  argument.description = description;
  return argument;
}

ArgumentBuilder<int>& ArgParser::AddIntArgument(char short_name, const std::string& long_name, const std::string& description) {
  auto& argument = AddIntArgument(short_name, long_name);
  argument.description = description;
  return argument;
}

int ArgParser::GetIntValue(const std::string& long_name) {
  return reinterpret_cast<ParserArgument<int>*>(arguments_[long_name])->GetValue();
}

int ArgParser::GetIntValue(const std::string& long_name, const std::size_t idx) {
  return reinterpret_cast<ParserArgument<int>*>(arguments_[long_name])->GetMultiValue(idx);
}

// --- Flag argument ---
ArgumentBuilder<bool>& ArgParser::AddFlag(const std::string& long_name) {
  auto* argument = CreateArgument<bool>();
  argument->long_name = long_name;
  long_value_types_[long_name] = ValueTypes::kFlag;
  argument_builders_.push_back(argument);
  return *argument;
}

ArgumentBuilder<bool>& ArgParser::AddFlag(const char short_name, const std::string& long_name) {
  auto& argument = AddFlag(long_name);
  argument.short_name = short_name;
  short_value_types_[short_name] = ValueTypes::kFlag;
  argument_builders_.push_back(&argument);
  return argument;
}

ArgumentBuilder<bool>& ArgParser::AddFlag(const std::string& long_name, const std::string& description) {
  auto& argument = AddFlag(long_name);
  argument.description = description;
  return argument;
}

ArgumentBuilder<bool>& ArgParser::AddFlag(const char short_name, const std::string& long_name, const std::string& description) {
  auto& argument = AddFlag(short_name, long_name);
  argument.description = description;
  return argument;
}

bool ArgParser::GetFlag(const std::string& long_name) {
  return reinterpret_cast<ParserArgument<bool>*>(arguments_[long_name])->GetValue();
}

// --- Help (me) ---
ArgumentBuilder<bool>& ArgParser::AddHelp(const char short_name, const std::string& long_name, const std::string& description) {
  auto* argument = CreateArgument<bool>();
  argument->short_name = short_name;
  argument->long_name = long_name;
  argument->description = "Display this help and exit";
  argument->is_help_ = true;

  description_ = description;

  // long_arguments_.flag_args.insert({long_name, argument});
  // short_arguments_.flag_args.insert({short_name, argument});
  argument_builders_.push_back(argument);
  long_value_types_[long_name] = ValueTypes::kFlag;
  return *argument;
}

bool ArgParser::Help() {
  return has_help_;
}

std::string ArgParser::HelpDescription() {
  return "";
  // std::string help_description = name_ + '\n';
  // help_description += description_ + "\n\n";
  // for (auto pair : long_arguments_.string_args) {
  //   if (pair.second->is_help_) {
  //     continue;
  //   }
  //   if (pair.second->short_name != '\0') {
  //     help_description += '-' + std::string(1, pair.second->short_name) + ",  ";
  //   } else {
  //     help_description += "     ";
  //   }
  //   help_description += "--" + pair.second->long_name + ",  " + pair.second->description + "\n";
  // }

  // for (auto pair : long_arguments_.int_args) {
  //   if (pair.second->short_name != '\0') {
  //     help_description += '-' + std::string(1, pair.second->short_name) + ",  ";
  //   } else {
  //     help_description += "     ";
  //   }
  //   help_description += "--" + pair.second->long_name + ",  " + pair.second->description + "\n";
  // }

  // ArgumentBuilder<bool>* help_argument = nullptr;

  // for (auto pair : long_arguments_.flag_args) {
  //   if (pair.second->is_help_) {
  //     help_argument = pair.second;
  //     continue;
  //   }
  //   if (pair.second->short_name != '\0') {
  //     help_description += '-' + std::string(1, pair.second->short_name) + ",  ";
  //   } else {
  //     help_description += "     ";
  //   }
  //   help_description += "--" + pair.second->long_name + ",  " + pair.second->description + "\n";
  // }

  // help_description +=
  //     "\n-" + std::string(1, help_argument->short_name) + ",  --" + help_argument->long_name + ",  " + help_argument->description;

  // return help_description;
}
