#pragma once
#include "Command.h"
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <vector>
class ConsoleInput {
private:
    std::map<std::string, GameCommand> keyMap;
    const std::vector<std::string> requiredCommands = {
        "MOVE_UP", "MOVE_DOWN", "MOVE_LEFT", "MOVE_RIGHT",
        "ATTACK", "SWITCH_MODE", "CAST_SPELL", "BUY_SPELL",
        "SAVE", "LOAD", "QUIT"
    };
    void loadDefaults();
    bool loadConfig(const std::string& filename);
    GameCommand parseCommandString(const std::string& cmdStr);
public:
    ConsoleInput();
    GameCommand getCommand();
};