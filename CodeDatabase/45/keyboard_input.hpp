#ifndef KEYBOARD_INPUT_HPP
#define KEYBOARD_INPUT_HPP

#include <iostream> 
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "command.hpp"
#include "input_master.hpp"

class KeyboardInput : public InputMaster{
public:
    ~KeyboardInput() = default;
    virtual Command get_string() override;
    virtual Command get_char() override;
};

#endif