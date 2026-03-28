#ifndef COMMAND_H
#define COMMAND_H

enum class CommandType {
    NONE,
    MOVE,
    ATTACK_RANGED,
    SWITCH_STYLE,
    CAST_SPELL,
    SAVE_GAME,
    QUIT,
    INVALID
};

struct Command {
    CommandType type;
    int targetX;
    int targetY;
    int value; // доп значение для индекса итд

    Command(CommandType t = CommandType::NONE, int x = 0, int y = 0, int v = 0)
        : type(t), targetX(x), targetY(y), value(v) {}
};

#endif