#pragma once
#include <string>
#include "action_type.h"

struct PlayerAction {
    ActionType type = ActionType::None;
    int dr = 0;
    int dc = 0;
    int target_r = 0;
    int target_c = 0;
    size_t spell_index = 0;
    std::string filename;
};