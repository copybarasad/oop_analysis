#pragma once

#include "enum_command_type.hpp"
#include <string>

class CommandManager{
private:
    CommandType type;
public:
    CommandManager(CommandType type);

    CommandType get_type();
};