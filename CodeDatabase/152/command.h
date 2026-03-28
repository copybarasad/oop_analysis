#pragma once
#include <string>
#include "command_type.h"

struct Command {
    CommandType type = CommandType::None;
    char dir = 0;
    int r = 0;
    int c = 0;
    size_t spell_index = 0;
    std::string filename;
};