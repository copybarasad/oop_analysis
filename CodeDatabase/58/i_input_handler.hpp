#pragma once

#include "command_manager.hpp"
#include <string>

class IRenderer;

class IInputHandler {
public:
    virtual ~IInputHandler() = default;

    virtual std::string read_input() = 0;
    virtual CommandManager parse_input(std::string input) = 0;
    virtual std::string get_input(std::string msg) = 0;
    virtual void setup_keybindings_interactive(IRenderer* renderer) = 0;
};