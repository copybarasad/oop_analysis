#ifndef CONSOLE_INPUT_HPP
#define CONSOLE_INPUT_HPP
#include <memory>
#include <string>
#include "control_keys.hpp"

class Command;
class Console_input{
    public:
        std::unique_ptr<Command> get_next_command(bool& is_help, bool& is_quit, int hand_size = 1);
    private:
        Control_keys control_keys;
        bool ask_position(int& x, int& y);
        bool ask_save_name(std::string& name);
};

#endif