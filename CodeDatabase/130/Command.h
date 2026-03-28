#ifndef COMMAND_H
#define COMMAND_H

enum class CommandType {
    NONE,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ATTACK,
    CAST_SPELL,
    SAVE,
    SWITCH_MODE,
    QUIT,
    INVALID
};

struct Command {
    CommandType type{CommandType::NONE};
    int dx{0};
    int dy{0};
    char raw{0};
};

#endif


