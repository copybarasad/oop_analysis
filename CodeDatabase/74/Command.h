#pragma once
#include <string>

enum class CommandType {
    MoveUp, MoveDown, MoveLeft, MoveRight,
    Action,
    Save,
    SwitchMode,
    Quit,
    OpenHand,
    SelectSpell,
    None
};

struct Command {
    CommandType type;
    int value;

    Command(CommandType t = CommandType::None, int v = 0) : type(t), value(v) {}
};