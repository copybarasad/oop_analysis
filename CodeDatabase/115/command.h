#ifndef COMMAND_H
#define COMMAND_H

#include <string>

enum class CommandType {
    None,
    Move,
    Attack,
    CastSpell,
    SwitchWeapon,
    OpenStore,
    Save,
    Load,           
    Quit
};

struct Command {
    CommandType type;
    std::string name;
    int dx = 0;
    int dy = 0;
};

#endif