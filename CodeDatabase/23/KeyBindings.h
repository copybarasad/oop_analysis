#ifndef KEY_BINDINGS_H
#define KEY_BINDINGS_H

#include <string>
#include <unordered_map>

enum class CommandType {
  MOVE_UP,
  MOVE_LEFT,
  MOVE_DOWN,
  MOVE_RIGHT,
  ATTACK_LEFT,
  ATTACK_UP,
  ATTACK_RIGHT,
  ATTACK_DOWN,
  SWITCH_ATTACK,
  CAST_SPELL,
  SAVE,
  SKIP,
  HELP,
  QUIT,
  UNKNOWN
};

class KeyBindings {
private:
  std::unordered_map<std::string, CommandType> keyToCommand;
  std::unordered_map<CommandType, std::string> commandToKey;

  void setDefaultBindings();
  bool loadFromFile(const std::string &filename);
  bool validate() const;

public:
  KeyBindings();
  explicit KeyBindings(const std::string &configFile);

  CommandType getCommandType(const std::string &key) const;
  std::string getKey(CommandType cmd) const;

  bool hasConfigFile(const std::string &filename) const;
};

#endif 
