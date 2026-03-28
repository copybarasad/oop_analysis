#ifndef KEYCONFIG_H
#define KEYCONFIG_H

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>


enum class CommandType {

    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,

    ATTACK_CLOSE,
    SWITCH_WEAPON,
    PAUSE_GAME,

    NEW_GAME,
    LOAD_GAME,
    QUIT_GAME,
    CONTINUE_LEVEL,

    MOUSE_CLICK,
    MOUSE_DROP,
    START_DRAG
};


class KeyConfig {
private:

    static KeyConfig* instance;
    KeyConfig();
    KeyConfig(const KeyConfig&) = delete;
    KeyConfig& operator=(const KeyConfig&) = delete;

    std::map<CommandType, int> keyBindings;
    std::map<int, CommandType> reverseBindings;
    bool configValid;


    std::string trim(const std::string& str) const;
    std::string toLower(const std::string& str) const;
    int stringToKeyCode(const std::string& keyStr) const;
    bool isValidKeyCode(int keyCode);
     std::map<CommandType, int> getDefaultBindings() const;

public:
    static KeyConfig& getInstance();
    ~KeyConfig() = default;
    std::string keyCodeToString(int keyCode) const;

    bool loadConfig(const std::string& filename = "keyconfig.cfg");
    bool saveConfig(const std::string& filename = "keyconfig.cfg");

    void resetToDefault();

    CommandType getCommand(int keyCode) const;
    int getKey(CommandType command) const;

    void setKeyBinding(CommandType command, int keyCode);

    bool isValid() const;

    static std::string commandToString(CommandType command);
    static std::vector<CommandType> getAllCommands();

    std::vector<std::string> validateConfig() const;
};

#endif // KEYCONFIG_H
