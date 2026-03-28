#ifndef CONSOLE_INPUT_READER_H
#define CONSOLE_INPUT_READER_H

#include <memory>
#include <string>
#include <iostream>
#include <cctype>

#include "command_interface.h"
#include "move_command.h"
#include "attack_command.h"
#include "switch_mode_command.h"
#include "cast_spell_command.h"
#include "save_command.h"
#include "load_command.h"
#include "exit_command.h"
#include "invalid_command.h"
#include "key_bindings.h"

class ConsoleInputReader
{
private:
    int max_spell_count;
    KeyBindings bindings;

    std::unique_ptr<ICommand> create_move_command(char key) const;
    std::unique_ptr<ICommand> create_spell_command(int spell_index) const;

public:
    ConsoleInputReader();
    explicit ConsoleInputReader(const std::string& config_path);

    void set_max_spell_count(int count);
    bool load_bindings(const std::string& filepath);
    std::string get_controls_help() const;
    
    std::unique_ptr<ICommand> read_command();
};

#endif