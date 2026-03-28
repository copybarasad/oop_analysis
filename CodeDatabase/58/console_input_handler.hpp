#pragma once

#include "enum_command_type.hpp"
#include "command_manager.hpp"
#include "i_input_handler.hpp"
#include "keybindings_manager.hpp"

#include <iostream>
#include <algorithm>
#include <limits>
#include <functional>
#include <unordered_map>
#include <string>

class ConsoleInputHandler : public IInputHandler{
private:
    std::unordered_map<std::string, CommandType> command_map;

    KeybindingsManager* keybindings_manager;
public:
    ConsoleInputHandler();
    ~ConsoleInputHandler();
    CommandManager parse_input(std::string input) override;
    std::string read_input() override;
    std::string get_input(std::string msg) override;

    void setup_keybindings_interactive(IRenderer* renderer) override;
};