#include "InputHandler.h"
#include <fstream>
#include <sstream>

InputHandler::InputHandler() {
  setupDefaultBindings();
}

InputHandler::InputHandler(const std::string& configFile) {
  loadBindingsFromFile(configFile);
}

void InputHandler::setupDefaultBindings() {
  keyBindings = {
    {'w', Command::MOVE_UP},
    {'s', Command::MOVE_DOWN},
    {'a', Command::MOVE_LEFT},
    {'d', Command::MOVE_RIGHT},
    {'m', Command::ATTACK_MELEE},
    {'r', Command::ATTACK_RANGED},
    {'t', Command::SWITCH_ATTACK},
    {'1', Command::CAST_SPELL_1},
    {'2', Command::CAST_SPELL_2},
    {'3', Command::CAST_SPELL_3},
    {'4', Command::CAST_SPELL_4},
    {'5', Command::CAST_SPELL_5},
    {'h', Command::SHOW_HELP},
    {'M', Command::SAVE_MENU},
    {'q', Command::QUIT_GAME}
  };
}

Command InputHandler::getCommand(char input) const {
  auto it = keyBindings.find(input);
  if (it != keyBindings.end()) {
    return it->second;
  }
  return Command::UNKNOWN;
}

void InputHandler::loadBindingsFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Cannot open config file, using default bindings" << std::endl;
    setupDefaultBindings();
    return;
  }
  
  std::unordered_map<char, Command> tempBindings;
  std::unordered_map<Command, char> reverseCheck;
  std::string line;
  
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string commandStr;
    char key;
    
    if (iss >> commandStr >> key) {
      Command cmd = Command::UNKNOWN;
      
      if (commandStr == "MOVE_UP") cmd = Command::MOVE_UP;
      else if (commandStr == "MOVE_DOWN") cmd = Command::MOVE_DOWN;
      else if (commandStr == "MOVE_LEFT") cmd = Command::MOVE_LEFT;
      else if (commandStr == "MOVE_RIGHT") cmd = Command::MOVE_RIGHT;
      else if (commandStr == "ATTACK_MELEE") cmd = Command::ATTACK_MELEE;
      else if (commandStr == "ATTACK_RANGED") cmd = Command::ATTACK_RANGED;
      else if (commandStr == "SWITCH_ATTACK") cmd = Command::SWITCH_ATTACK;
      else if (commandStr == "CAST_SPELL_1") cmd = Command::CAST_SPELL_1;
      else if (commandStr == "CAST_SPELL_2") cmd = Command::CAST_SPELL_2;
      else if (commandStr == "CAST_SPELL_3") cmd = Command::CAST_SPELL_3;
      else if (commandStr == "CAST_SPELL_4") cmd = Command::CAST_SPELL_4;
      else if (commandStr == "CAST_SPELL_5") cmd = Command::CAST_SPELL_5;
      else if (commandStr == "SHOW_HELP") cmd = Command::SHOW_HELP;
      else if (commandStr == "SAVE_MENU") cmd = Command::SAVE_MENU;
      else if (commandStr == "QUIT_GAME") cmd = Command::QUIT_GAME;
      
      if (cmd != Command::UNKNOWN) {
        if (tempBindings.find(key) != tempBindings.end()) {
          std::cerr << "Config error: key '" << key << "' has multiple commands" << std::endl;
          setupDefaultBindings();
          return;
        }
        
        if (reverseCheck.find(cmd) != reverseCheck.end()) {
          std::cerr << "Config error: command '" << commandStr << "' has multiple keys" << std::endl;
          setupDefaultBindings();
          return;
        }
        
        tempBindings[key] = cmd;
        reverseCheck[cmd] = key;
      }
    }
  }
  
  bool allCommandsSet = true;
  for (int i = 0; i < static_cast<int>(Command::UNKNOWN); i++) {
    Command cmd = static_cast<Command>(i);
    if (reverseCheck.find(cmd) == reverseCheck.end()) {
      std::cerr << "Config error: missing binding for some commands" << std::endl;
      allCommandsSet = false;
      break;
    }
  }
  
  if (allCommandsSet) {
    keyBindings = tempBindings;
    std::cout << "Bindings loaded from config file" << std::endl;
  } else {
    std::cerr << "Using default bindings" << std::endl;
    setupDefaultBindings();
  }
}

void InputHandler::printCurrentBindings() const {
  std::cout << "\n=== Current Key Bindings ===" << std::endl;
  for (const auto& binding : keyBindings) {
    std::cout << "'" << binding.first << "' -> ";
    
    switch(binding.second) {
      case Command::MOVE_UP: std::cout << "MOVE_UP"; break;
      case Command::MOVE_DOWN: std::cout << "MOVE_DOWN"; break;
      case Command::MOVE_LEFT: std::cout << "MOVE_LEFT"; break;
      case Command::MOVE_RIGHT: std::cout << "MOVE_RIGHT"; break;
      case Command::ATTACK_MELEE: std::cout << "ATTACK_MELEE"; break;
      case Command::ATTACK_RANGED: std::cout << "ATTACK_RANGED"; break;
      case Command::SWITCH_ATTACK: std::cout << "SWITCH_ATTACK"; break;
      case Command::CAST_SPELL_1: std::cout << "CAST_SPELL_1"; break;
      case Command::CAST_SPELL_2: std::cout << "CAST_SPELL_2"; break;
      case Command::CAST_SPELL_3: std::cout << "CAST_SPELL_3"; break;
      case Command::CAST_SPELL_4: std::cout << "CAST_SPELL_4"; break;
      case Command::CAST_SPELL_5: std::cout << "CAST_SPELL_5"; break;
      case Command::SHOW_HELP: std::cout << "SHOW_HELP"; break;
      case Command::SAVE_MENU: std::cout << "SAVE_MENU"; break;
      case Command::QUIT_GAME: std::cout << "QUIT_GAME"; break;
      case Command::UNKNOWN: std::cout << "UNKNOWN"; break;
    }
    std::cout << std::endl;
  }
}