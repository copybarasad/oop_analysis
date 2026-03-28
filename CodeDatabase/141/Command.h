#pragma once

#include <optional>

enum class CommandType {
    NONE,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ATTACK,
    SWITCH_WEAPON,
    CAST_SPELL,
    SAVE_GAME,
    LOAD_GAME,
    EXIT,
    HELP
};

struct Direction {
    int dx;
    int dy;
};

struct Command {
    CommandType type{CommandType::NONE};
    std::optional<Direction> direction{};
    std::optional<int> index{};

    bool requiresDirection() const {
        return type == CommandType::MOVE_UP || type == CommandType::MOVE_DOWN ||
               type == CommandType::MOVE_LEFT || type == CommandType::MOVE_RIGHT ||
               type == CommandType::ATTACK || type == CommandType::CAST_SPELL;
    }
};
