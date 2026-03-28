#include "KeyBindings.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <set>

using json = nlohmann::json;

KeyBindings::KeyBindings() { setDefaultBindings(); }

KeyBindings::KeyBindings(const std::string &configFile) {
  if (!loadFromFile(configFile)) {
    std::cout << "Warning: Could not load key bindings from " << configFile
              << std::endl;
    std::cout << "Using default key bindings." << std::endl;
    setDefaultBindings();
  } else if (!validate()) {
    std::cout << "Warning: Invalid key bindings configuration!" << std::endl;
    std::cout << "Using default key bindings." << std::endl;
    setDefaultBindings();
  }
}

void KeyBindings::setDefaultBindings() {
  keyToCommand.clear();
  commandToKey.clear();

  
  keyToCommand["w"] = CommandType::MOVE_UP;
  keyToCommand["a"] = CommandType::MOVE_LEFT;
  keyToCommand["s"] = CommandType::MOVE_DOWN;
  keyToCommand["d"] = CommandType::MOVE_RIGHT;
  keyToCommand["h"] = CommandType::ATTACK_LEFT;
  keyToCommand["j"] = CommandType::ATTACK_UP;
  keyToCommand["k"] = CommandType::ATTACK_RIGHT;
  keyToCommand["l"] = CommandType::ATTACK_DOWN;
  keyToCommand["switch"] = CommandType::SWITCH_ATTACK;
  keyToCommand["cast"] = CommandType::CAST_SPELL;
  keyToCommand["save"] = CommandType::SAVE;
  keyToCommand["skip"] = CommandType::SKIP;
  keyToCommand["help"] = CommandType::HELP;
  keyToCommand["quit"] = CommandType::QUIT;

  
  for (const auto &pair : keyToCommand) {
    commandToKey[pair.second] = pair.first;
  }
}

bool KeyBindings::loadFromFile(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  try {
    json j;
    file >> j;

    keyToCommand.clear();
    commandToKey.clear();

    
    if (j.contains("MOVE_UP") && j["MOVE_UP"].is_string()) {
      std::string key = j["MOVE_UP"];
      keyToCommand[key] = CommandType::MOVE_UP;
    }
    if (j.contains("MOVE_LEFT") && j["MOVE_LEFT"].is_string()) {
      std::string key = j["MOVE_LEFT"];
      keyToCommand[key] = CommandType::MOVE_LEFT;
    }
    if (j.contains("MOVE_DOWN") && j["MOVE_DOWN"].is_string()) {
      std::string key = j["MOVE_DOWN"];
      keyToCommand[key] = CommandType::MOVE_DOWN;
    }
    if (j.contains("MOVE_RIGHT") && j["MOVE_RIGHT"].is_string()) {
      std::string key = j["MOVE_RIGHT"];
      keyToCommand[key] = CommandType::MOVE_RIGHT;
    }
    if (j.contains("ATTACK_LEFT") && j["ATTACK_LEFT"].is_string()) {
      std::string key = j["ATTACK_LEFT"];
      keyToCommand[key] = CommandType::ATTACK_LEFT;
    }
    if (j.contains("ATTACK_UP") && j["ATTACK_UP"].is_string()) {
      std::string key = j["ATTACK_UP"];
      keyToCommand[key] = CommandType::ATTACK_UP;
    }
    if (j.contains("ATTACK_RIGHT") && j["ATTACK_RIGHT"].is_string()) {
      std::string key = j["ATTACK_RIGHT"];
      keyToCommand[key] = CommandType::ATTACK_RIGHT;
    }
    if (j.contains("ATTACK_DOWN") && j["ATTACK_DOWN"].is_string()) {
      std::string key = j["ATTACK_DOWN"];
      keyToCommand[key] = CommandType::ATTACK_DOWN;
    }
    if (j.contains("SWITCH_ATTACK") && j["SWITCH_ATTACK"].is_string()) {
      std::string key = j["SWITCH_ATTACK"];
      keyToCommand[key] = CommandType::SWITCH_ATTACK;
    }
    if (j.contains("CAST_SPELL") && j["CAST_SPELL"].is_string()) {
      std::string key = j["CAST_SPELL"];
      keyToCommand[key] = CommandType::CAST_SPELL;
    }
    if (j.contains("SAVE") && j["SAVE"].is_string()) {
      std::string key = j["SAVE"];
      keyToCommand[key] = CommandType::SAVE;
    }
    if (j.contains("SKIP") && j["SKIP"].is_string()) {
      std::string key = j["SKIP"];
      keyToCommand[key] = CommandType::SKIP;
    }
    if (j.contains("HELP") && j["HELP"].is_string()) {
      std::string key = j["HELP"];
      keyToCommand[key] = CommandType::HELP;
    }
    if (j.contains("QUIT") && j["QUIT"].is_string()) {
      std::string key = j["QUIT"];
      keyToCommand[key] = CommandType::QUIT;
    }

    
    for (const auto &pair : keyToCommand) {
      commandToKey[pair.second] = pair.first;
    }

    return true;
  } catch (const std::exception &e) {
    std::cerr << "Error parsing JSON config: " << e.what() << std::endl;
    return false;
  }
}

bool KeyBindings::validate() const {
  
  std::vector<CommandType> requiredCommands = {
      CommandType::MOVE_UP,       CommandType::MOVE_LEFT,
      CommandType::MOVE_DOWN,     CommandType::MOVE_RIGHT,
      CommandType::ATTACK_LEFT,   CommandType::ATTACK_UP,
      CommandType::ATTACK_RIGHT,  CommandType::ATTACK_DOWN,
      CommandType::SWITCH_ATTACK, CommandType::CAST_SPELL,
      CommandType::SAVE,          CommandType::SKIP,
      CommandType::HELP,          CommandType::QUIT};

  for (CommandType cmd : requiredCommands) {
    auto it = commandToKey.find(cmd);
    if (it == commandToKey.end()) {
      std::cout << "Warning: Missing command binding in config file"
                << std::endl;
      return false;
    }

    
    if (it->second.empty()) {
      std::cout << "Warning: Empty key binding found in config file"
                << std::endl;
      return false;
    }
  }

  
  std::unordered_map<std::string, int> keyCount;
  for (const auto &pair : keyToCommand) {
    keyCount[pair.first]++;
    if (keyCount[pair.first] > 1) {
      std::cout << "Warning: Duplicate key '" << pair.first
                << "' found in config file" << std::endl;
      return false;
    }
  }

  
  std::unordered_map<CommandType, int> cmdCount;
  for (const auto &pair : commandToKey) {
    cmdCount[pair.first]++;
    if (cmdCount[pair.first] > 1) {
      std::cout << "Warning: Duplicate command binding found in config file"
                << std::endl;
      return false;
    }
  }

  return true;
}

CommandType KeyBindings::getCommandType(const std::string &key) const {
  auto it = keyToCommand.find(key);
  if (it != keyToCommand.end()) {
    return it->second;
  }
  return CommandType::UNKNOWN;
}

std::string KeyBindings::getKey(CommandType cmd) const {
  auto it = commandToKey.find(cmd);
  if (it != commandToKey.end()) {
    return it->second;
  }
  return "";
}

bool KeyBindings::hasConfigFile(const std::string &filename) const {
  std::ifstream file(filename);
  return file.good();
}
