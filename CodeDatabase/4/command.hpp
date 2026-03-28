#pragma once
#include <string>

enum class GameAction {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    CAST_SPELL,
    SWAP_WEAPON,
    BUY_SPELL,
    QUIT,
    SAVE,
    INVALID
};

struct Command {
    GameAction action;
    int spell_index = -1;
    int target_x = -1;
    int target_y = -1;
    std::string save_filename;

    Command(GameAction a);
    Command(GameAction a, int idx);
    Command(GameAction a, int tx, int ty);
    Command(GameAction a, int idx, int tx, int ty);
    Command(GameAction a, const std::string& name);
};