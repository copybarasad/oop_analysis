#ifndef UNTITLED_INPUTHANDLER_H
#define UNTITLED_INPUTHANDLER_H

#include <optional>
#include <string>
#include "Coord.h"

enum class CommandType {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    SwitchMode,
    ShowHand,
    Cast,
    Save,
    Load,
    Quit,
    None
};

struct Command {
    CommandType type = CommandType::None;
    size_t castIndex = 0;
    std::optional<Coord> target;
    std::string arg;
};


#endif //UNTITLED_INPUTHANDLER_H
