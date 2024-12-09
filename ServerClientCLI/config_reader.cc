#include "config_reader.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>

namespace printconfig {
const std::string kConfigFile = "~/.printconfig";

// Remove leading and trailing white space
std::string RemoveWhitespace(std::string str) {
  // Remove leading space
  while (str[0] == ' ' || str[0] == '\t') {
    str = str.substr(1);
  }

  // Remove trailing space
  while(str[str.size() - 1] == ' ' || str[str.size() - 1] == '\t') {
    str = str.substr(0, str.size() - 1);
  }

  return str;
}

std::string resolvePath(const std::string& path) {
  if (!path.empty() && path[0] == '~') {
    const char* home = std::getenv("HOME");
    if (home) {
      return std::string(home) + path.substr(1); // Replace '~' with the HOME directory
    } else {
      throw std::runtime_error("HOME environment variable is not set.");
    }
  }
  return path;
}

std::string ReplaceVariables(std::string line, const std::map<std::string, std::string>& variables) {
  std::regex variablePattern(R"(\$\((\w+)\))"); // Matches $(VARIABLE_NAME)
  std::smatch match;

  while (std::regex_search(line, match, variablePattern)) {
    std::string variableName = match[1]; // Extract variable name
    if (variables.count(variableName)) {
      // Replace $(VARIABLE_NAME) with its value
      line.replace(match.position(0), match.length(0), variables.at(variableName));
    } else {
      throw std::runtime_error("Undefined variable: " + variableName);
    }
  }
  return line;
}

std::map<std::string, std::string> ReadProperties() {
  std::string config_path = resolvePath(kConfigFile);
  // Check if the file exists in the current directory
  if (!std::filesystem::exists(config_path)) {
    throw std::runtime_error("Properties file does not exist: " + config_path);
  }

  // Open the file
  std::ifstream file(config_path);
  if (!file.is_open()) {
    throw std::runtime_error("Unable to open properties file: " + config_path);
  }

  std::map<std::string, std::string> properties;
  std::string line;

  while (std::getline(file, line)) {
    // Skip comments and empty lines
    if (line.empty() || line[0] == '#') {
      continue;
    }

    size_t delimiterPos = line.find('=');
    if (delimiterPos != std::string::npos) {
      std::string key = line.substr(0, delimiterPos);
      std::string value = line.substr(delimiterPos + 1);

      key = RemoveWhitespace(key);
      value = RemoveWhitespace(value);
      value = ReplaceVariables(value, properties);

      properties[key] = value;
    }
  }
  return properties;
}

}