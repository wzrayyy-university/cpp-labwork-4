#include "ArgParser.h"

using namespace ArgumentParser;

// --- ArgParser ---
ArgParser::ArgParser(const std::string& name) : name_(name){};

// --- Parse ---
bool ArgParser::Parse(const std::vector<std::string>& argv) {
  std::vector<NormalizedArgument> normalized = NormalizeArguments(argv);

  for (std::size_t i = 0; i < normalized.size(); ++i) {
    if (normalized[i].type == ArgumentType::kLong) {
      if (long_value_types_[normalized[i].long_name] == ValueTypes::kString) {
        long_arguments_.string_args[normalized[i].long_name]->SetValue(normalized[i].value);
      } else if (long_value_types_[normalized[i].long_name] == ValueTypes::kInt) {
        long_arguments_.int_args[normalized[i].long_name]->SetValue(std::stoi(normalized[i].value));
      } else if (long_value_types_[normalized[i].long_name] == ValueTypes::kFlag) {
        if (long_arguments_.flag_args[normalized[i].long_name]->is_help_ == true) { // ugh 2
          has_help_ = true;
          return true;
        }
        long_arguments_.flag_args[normalized[i].long_name]->SetValue(true);
      }
    } else if (normalized[i].type == ArgumentType::kShort) {
      if (short_value_types_[normalized[i].short_name] == ValueTypes::kString) {
        short_arguments_.string_args[normalized[i].short_name]->SetValue(normalized[i].value);
      } else if (short_value_types_[normalized[i].short_name] == ValueTypes::kInt) {
        short_arguments_.int_args[normalized[i].short_name]->SetValue(std::stoi(normalized[i].value));
      } else if (short_value_types_[normalized[i].short_name] == ValueTypes::kFlag) {
        if (short_arguments_.flag_args[normalized[i].short_name]->is_help_ == true) {
          has_help_ = true;
          return true;
        }
        short_arguments_.flag_args[normalized[i].short_name]->SetValue(true);
      }
    } else if (normalized[i].type == ArgumentType::kPositional) {
      std::string name = FindPositionalArgumentName();
      if (long_value_types_[name] == ValueTypes::kString) {
        long_arguments_.string_args[name]->SetValue(normalized[i].value);
      } else if (long_value_types_[name] == ValueTypes::kInt) {
        long_arguments_.int_args[name]->SetValue(std::stoi(normalized[i].value));
      } else if (long_value_types_[name] == ValueTypes::kFlag) {
        long_arguments_.flag_args[name]->SetValue(true);
      }
    }
  }
  return CheckForInvalidArgs();
};

bool ArgParser::Parse(int argc, char** argv) {
  return Parse(std::vector<std::string>(argv, argv + argc));
}

// -- Has invalid arguments --
template<typename T> // ugh (i hope that i won't have to redo this in class 🙏)
bool CheckForInvalidArgs_(std::map<std::string, ParserArgument<T>*> args) {
  for (auto pair : args) {
    if ((!pair.second->has_default_ && !pair.second->has_value_) && !pair.second->is_help_) {
      return false;
    }
    if (pair.second->GetMultiValueSize() < pair.second->min_args_count_) {
      return false;
    }
  }
  return true;
}

bool ArgParser::CheckForInvalidArgs() {
  return CheckForInvalidArgs_<std::string>(long_arguments_.string_args) && CheckForInvalidArgs_<int>(long_arguments_.int_args)
      && CheckForInvalidArgs_<bool>(long_arguments_.flag_args);
}

// --- Find positional argument name ---
std::string ArgParser::FindPositionalArgumentName() {
  for (auto pair : long_arguments_.string_args) {
    if (pair.second->is_positional_) {
      return pair.first;
    }
  }
  for (auto pair : long_arguments_.int_args) {
    if (pair.second->is_positional_) {
      return pair.first;
    }
  }
  for (auto pair : long_arguments_.flag_args) {
    if (pair.second->is_positional_) {
      return pair.first;
    }
  }

  return "";
}

// --- Create argument ---
template<typename T>
ParserArgument<T>* ArgParser::CreateArgument() {
  ParserArgument<T>* argument = new ParserArgument<T>;
  return argument;
}

// --- String argument ---
ParserArgument<std::string>& ArgParser::AddStringArgument(const std::string& long_name) {
  auto* argument = CreateArgument<std::string>();
  argument->long_name = long_name;
  long_value_types_[long_name] = ValueTypes::kString;
  long_arguments_.string_args.insert({long_name, argument});
  return *argument;
}

ParserArgument<std::string>& ArgParser::AddStringArgument(const char short_name, const std::string& long_name) {
  auto* argument = &AddStringArgument(long_name);
  short_value_types_[short_name] = ValueTypes::kString;
  argument->short_name = short_name;
  short_arguments_.string_args.insert({short_name, argument});
  return *argument;
}

ParserArgument<std::string>& ArgParser::AddStringArgument(const std::string& long_name, const std::string& description) {
  auto* argument = &AddStringArgument(long_name);
  argument->description = description;
  return *argument;
}

ParserArgument<std::string>& ArgParser::AddStringArgument(const char short_name, const std::string& long_name,
                                                          const std::string& description) {
  auto* argument = &AddStringArgument(short_name, long_name);
  argument->description = description;
  return *argument;
}

std::string ArgParser::GetStringValue(const std::string& long_name) {
  return long_arguments_.string_args[long_name]->GetValue();
}

std::string ArgParser::GetStringValue(const std::string& long_name, const std::size_t idx) {
  return long_arguments_.string_args[long_name]->GetMultiValue(idx);
}

// --- Int argument ---
ParserArgument<int>& ArgParser::AddIntArgument(const std::string& long_name) {
  auto* argument = CreateArgument<int>();
  argument->long_name = long_name;
  long_value_types_[long_name] = ValueTypes::kInt;
  long_arguments_.int_args.insert({long_name, argument});
  return *argument;
}

ParserArgument<int>& ArgParser::AddIntArgument(char short_name, const std::string& long_name) {
  auto& argument = AddIntArgument(long_name);
  argument.short_name = short_name;
  short_value_types_[short_name] = ValueTypes::kInt;
  short_arguments_.int_args.insert({short_name, &argument});
  return argument;
}

ParserArgument<int>& ArgParser::AddIntArgument(const std::string& long_name, const std::string& description) {
  auto& argument = AddIntArgument(long_name);
  argument.description = description;
  return argument;
}

ParserArgument<int>& ArgParser::AddIntArgument(char short_name, const std::string& long_name, const std::string& description) {
  auto& argument = AddIntArgument(short_name, long_name);
  argument.description = description;
  return argument;
}

int ArgParser::GetIntValue(const std::string& long_name) {
  return long_arguments_.int_args[long_name]->GetValue();
}

int ArgParser::GetIntValue(const std::string& long_name, const std::size_t idx) {
  return long_arguments_.int_args[long_name]->GetMultiValue(idx);
}

// --- Flag argument ---
ParserArgument<bool>& ArgParser::AddFlag(const std::string& long_name) {
  auto* argument = CreateArgument<bool>();
  argument->long_name = long_name;
  long_value_types_[long_name] = ValueTypes::kFlag;
  long_arguments_.flag_args.insert({long_name, argument});
  return *argument;
}

ParserArgument<bool>& ArgParser::AddFlag(const char short_name, const std::string& long_name) {
  auto& argument = AddFlag(long_name);
  argument.short_name = short_name;
  short_value_types_[short_name] = ValueTypes::kFlag;
  short_arguments_.flag_args.insert({short_name, &argument});
  return argument;
}

ParserArgument<bool>& ArgParser::AddFlag(const std::string& long_name, const std::string& description) {
  auto& argument = AddFlag(long_name);
  argument.description = description;
  return argument;
}

ParserArgument<bool>& ArgParser::AddFlag(const char short_name, const std::string& long_name, const std::string& description) {
  auto& argument = AddFlag(short_name, long_name);
  argument.description = description;
  return argument;
}

bool ArgParser::GetFlag(const std::string& long_name) {
  return long_arguments_.flag_args[long_name]->GetValue();
}

// TODO --- Help (me) ---
ParserArgument<bool>& ArgParser::AddHelp(const char short_name, const std::string& long_name, const std::string& description) {
  auto* argument = CreateArgument<bool>();
  argument->short_name = short_name;
  argument->long_name = long_name;
  argument->description = "Display this help and exit";
  argument->is_help_ = true;

  description_ = description;

  long_arguments_.flag_args.insert({long_name, argument});
  short_arguments_.flag_args.insert({short_name, argument});
  long_value_types_[long_name] = ValueTypes::kFlag;
  return *argument;
}

bool ArgParser::Help() {
  return has_help_;
}

std::string ArgParser::HelpDescription() {
  std::string help_description = name_ + '\n';
  help_description += description_ + "\n\n";
  for (auto pair : long_arguments_.string_args) {
    if (pair.second->is_help_) {
      continue;
    }
    if (pair.second->short_name != '\0') {
      help_description += '-' + std::string(1, pair.second->short_name) + ",  ";
    } else {
      help_description += "     ";
    }
    help_description += "--" + pair.second->long_name + ",  " + pair.second->description + "\n";
  }

  for (auto pair : long_arguments_.int_args) {
    if (pair.second->short_name != '\0') {
      help_description += '-' + std::string(1, pair.second->short_name) + ",  ";
    } else {
      help_description += "     ";
    }
    help_description += "--" + pair.second->long_name + ",  " + pair.second->description + "\n";
  }

  ParserArgument<bool>* help_argument = nullptr;

  for (auto pair : long_arguments_.flag_args) {
    if (pair.second->is_help_) {
      help_argument = pair.second;
      continue;
    }
    if (pair.second->short_name != '\0') {
      help_description += '-' + std::string(1, pair.second->short_name) + ",  ";
    } else {
      help_description += "     ";
    }
    help_description += "--" + pair.second->long_name + ",  " + pair.second->description + "\n";
  }

  help_description +=
      "\n-" + std::string(1, help_argument->short_name) + ",  --" + help_argument->long_name + ",  " + help_argument->description;

  return help_description;
}
