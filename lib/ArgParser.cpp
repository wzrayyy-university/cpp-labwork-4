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
};

bool ArgParser::Parse(int argc, char** argv) {
  return Parse(std::vector<std::string>(argv, argv + argc));
}

// -- Has invalid arguments --
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

// --- Find positional argument name ---
std::string ArgParser::FindPositionalArgumentName() {
  for (auto pair : arguments_) {
    if (pair.second->is_positional_) {
      return pair.first;
    }
  }

  return "";
}

// --- String argument ---
ArgumentBuilder<std::string>& ArgParser::AddStringArgument(const std::string& long_name) {
  auto* builder = new ArgumentBuilder<std::string>;
  builder->long_name = long_name;
  long_value_types_[long_name] = ValueTypes::kString;
  argument_builders_.push_back(builder);
  return *builder;
}

ArgumentBuilder<std::string>& ArgParser::AddStringArgument(const char short_name, const std::string& long_name) {
  auto* builder = &AddStringArgument(long_name);
  short_value_types_[short_name] = ValueTypes::kString;
  builder->short_name = short_name;
  argument_builders_.push_back(builder);
  return *builder;
}

ArgumentBuilder<std::string>& ArgParser::AddStringArgument(const std::string& long_name, const std::string& description) {
  auto* builder = &AddStringArgument(long_name);
  builder->description = description;
  return *builder;
}

ArgumentBuilder<std::string>& ArgParser::AddStringArgument(const char short_name, const std::string& long_name,
                                                           const std::string& description) {
  auto* builder = &AddStringArgument(short_name, long_name);
  builder->description = description;
  return *builder;
}

std::string ArgParser::GetStringValue(const std::string& long_name) {
  return reinterpret_cast<ParserArgument<std::string>*>(arguments_[long_name])->GetValue();
}

std::string ArgParser::GetStringValue(const std::string& long_name, const std::size_t idx) {
  return reinterpret_cast<ParserArgument<std::string>*>(arguments_[long_name])->GetMultiValue(idx);
}

// --- Int argument ---
ArgumentBuilder<int>& ArgParser::AddIntArgument(const std::string& long_name) {
  auto* builder = new ArgumentBuilder<int>;
  builder->long_name = long_name;
  long_value_types_[long_name] = ValueTypes::kInt;
  argument_builders_.push_back(builder);
  return *builder;
}

ArgumentBuilder<int>& ArgParser::AddIntArgument(char short_name, const std::string& long_name) {
  auto& builder = AddIntArgument(long_name);
  builder.short_name = short_name;
  short_value_types_[short_name] = ValueTypes::kInt;
  argument_builders_.push_back(&builder);
  return builder;
}

ArgumentBuilder<int>& ArgParser::AddIntArgument(const std::string& long_name, const std::string& description) {
  auto& builder = AddIntArgument(long_name);
  builder.description = description;
  return builder;
}

ArgumentBuilder<int>& ArgParser::AddIntArgument(char short_name, const std::string& long_name, const std::string& description) {
  auto& builder = AddIntArgument(short_name, long_name);
  builder.description = description;
  return builder;
}

int ArgParser::GetIntValue(const std::string& long_name) {
  return reinterpret_cast<ParserArgument<int>*>(arguments_[long_name])->GetValue();
}

int ArgParser::GetIntValue(const std::string& long_name, const std::size_t idx) {
  return reinterpret_cast<ParserArgument<int>*>(arguments_[long_name])->GetMultiValue(idx);
}

// --- Flag argument ---
ArgumentBuilder<bool>& ArgParser::AddFlag(const std::string& long_name) {
  auto* builder = new ArgumentBuilder<bool>;
  builder->long_name = long_name;
  long_value_types_[long_name] = ValueTypes::kFlag;
  argument_builders_.push_back(builder);
  return *builder;
}

ArgumentBuilder<bool>& ArgParser::AddFlag(const char short_name, const std::string& long_name) {
  auto& builder = AddFlag(long_name);
  builder.short_name = short_name;
  short_value_types_[short_name] = ValueTypes::kFlag;
  argument_builders_.push_back(&builder);
  return builder;
}

ArgumentBuilder<bool>& ArgParser::AddFlag(const std::string& long_name, const std::string& description) {
  auto& builder = AddFlag(long_name);
  builder.description = description;
  return builder;
}

ArgumentBuilder<bool>& ArgParser::AddFlag(const char short_name, const std::string& long_name, const std::string& description) {
  auto& builder = AddFlag(short_name, long_name);
  builder.description = description;
  return builder;
}

bool ArgParser::GetFlag(const std::string& long_name) {
  return reinterpret_cast<ParserArgument<bool>*>(arguments_[long_name])->GetValue();
}

// --- Help (me) ---
ArgumentBuilder<bool>& ArgParser::AddHelp(const char short_name, const std::string& long_name, const std::string& description) {
  auto* builder = new ArgumentBuilder<bool>;
  builder->short_name = short_name;
  builder->long_name = long_name;
  builder->description = "Display this help and exit";
  builder->is_help_ = true;

  description_ = description;

  argument_builders_.push_back(builder);
  long_value_types_[long_name] = ValueTypes::kFlag;
  return *builder;
}

bool ArgParser::Help() {
  return has_help_;
}

std::string ArgParser::HelpDescription() {
  std::string help_description = name_ + '\n';
  help_description += description_ + "\n\n";

  IParserArgument* help_argument = nullptr;

  for (auto pair : arguments_) {
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
