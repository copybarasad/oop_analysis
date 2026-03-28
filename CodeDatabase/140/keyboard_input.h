#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>

#include "game_command.h"

enum class LogicalCommand {
    Quit, Help, MoveUp, MoveDown, MoveLeft, MoveRight,
    SwitchMode, Attack, UseSpell, Save, Load, Invalid
};

struct KeyBindings {
    std::map<std::string, LogicalCommand> keyToCommand;
    std::map<LogicalCommand, std::string> commandToKey;
};


class KeyboardInput {
public:
    KeyboardInput() {
        if (!load_from_file("controls.cfg")) {
            set_default();
        }
    }

    GameCommand get_next_command();
private:
    KeyBindings bindings;

    void add_binding(const std::string& key, LogicalCommand cmd);
    void set_default();

    LogicalCommand parse_command(const std::string& s);
    bool load_from_file(const std::string& filename);
};
