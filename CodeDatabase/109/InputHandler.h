#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <iostream>
#include <string>
#include <unordered_map>

enum class Command {
  MOVE_UP,
  MOVE_DOWN,
  MOVE_LEFT,
  MOVE_RIGHT,
  ATTACK_MELEE,
  ATTACK_RANGED,
  SWITCH_ATTACK,
  CAST_SPELL_1,
  CAST_SPELL_2,
  CAST_SPELL_3,
  CAST_SPELL_4,
  CAST_SPELL_5,
  SHOW_HELP,
  SAVE_MENU,
  QUIT_GAME,
  UNKNOWN
};

class InputHandler {
private:
  std::unordered_map<char, Command> keyBindings;
  
  void setupDefaultBindings();
  
public:
  InputHandler();
  InputHandler(const std::string& configFile);
  
  Command getCommand(char input) const;
  void loadBindingsFromFile(const std::string& filename);
  void printCurrentBindings() const;
};

#endif