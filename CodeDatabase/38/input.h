#pragma once
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <string>
#include <unordered_map>

enum class COMMAND {
  NOTHING,
  MOVED_UP,
  MOVED_DOWN,
  MOVED_LEFT,
  MOVED_RIGHT,
  REJECT_MOVE,
  DIRECTSPELL,
  AREASPELL,
  TRAPSPELL,
  UNSET
};

class Converter {
private:
  std::unordered_map<KeyboardKey, COMMAND> keytoCommand;
  std::unordered_map<COMMAND, KeyboardKey> commandtoKey;

  std::unordered_map<COMMAND, KeyboardKey> defaults = {
      {COMMAND::MOVED_UP, KEY_W},    {COMMAND::MOVED_DOWN, KEY_S},
      {COMMAND::MOVED_LEFT, KEY_A},  {COMMAND::MOVED_RIGHT, KEY_D},
      {COMMAND::DIRECTSPELL, KEY_E}, {COMMAND::AREASPELL, KEY_R},
      {COMMAND::TRAPSPELL, KEY_T}};

public:
  Converter(const std::string &config_file = "controls.cfg");
  // COMMAND ProcessMove(const char keypress);
  COMMAND ProcessMove() const; // KeyboardKey from raylib
  KeyboardKey readKey();
  bool loadConfig(const std::string &filename = "controls.cfg");
  bool checkConfig() const;
  void setDefaultMaps();

  void reloadConfig(const std::string &config_file = "controls.cfg");
  std::string getKeyName(COMMAND cmd) const;
  std::unordered_map<COMMAND, KeyboardKey> getCurrentMappings() const;

private:
  KeyboardKey stringToKey(const std::string &key_str) const;
  std::string keyToStr(KeyboardKey key) const;
};
