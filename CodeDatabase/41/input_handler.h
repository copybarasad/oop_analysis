#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "command.h"
#include <map>
#include <memory>
#include <string>
#include <iostream>

class Input_handler{
protected:
    std::map<char, std::unique_ptr<Command>> key_bindings;
    void setup_default_bindings();
public:
    Input_handler();
    virtual ~Input_handler() = default;
    virtual std::unique_ptr<Command> get_next_command() = 0;
    bool load_bindings_from_file(const std::string& file_name);
    void set_binding(char key, std::unique_ptr<Command> command);
    char get_key_for_command(const std::string& command_name) const;
};

class Console_input_handler : public Input_handler{
public:
    Console_input_handler();
    std::unique_ptr<Command> get_next_command() override;
};

#endif

