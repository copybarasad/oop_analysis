#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <cstddef>

enum class Action {
    None,
    Move,
    Attack,
    Use,
    Save,
    Load,
    Next,
    Quit
};

struct Command {
    Action action = Action::None;
    int x = 0;
    int y = 0;
    std::size_t idx = 0;
    std::string arg;
};

#endif // COMMAND_H
