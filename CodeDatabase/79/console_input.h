#ifndef CONSOLE_INPUT_H
#define CONSOLE_INPUT_H

#include <memory>

#include "commands.h"

class Console_input{
public:
    char input_command();
    std::unique_ptr<Basic_command> create_level_command();
    std::unique_ptr<Basic_command> create_main_menu_command();
    std::unique_ptr<Basic_command> create_upgrade_menu_command();
    // ~Console_input();
};


#endif