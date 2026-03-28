#ifndef COMMAND_H
#define COMMAND_H

#include <optional>
#include <string>
#include <utility>

struct Command {
    std::string action;  // Было "com", должно быть "action"
    std::optional<std::string> spell_name;
    std::optional<std::pair<int, int>> coordinates;
    std::optional<std::string> filename;
};

#endif