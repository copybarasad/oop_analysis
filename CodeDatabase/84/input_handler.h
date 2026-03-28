#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "command.h"
#include <map>
#include <string>
#include <memory>

class InputHandler {
private:
    std::map<char, std::string> key_bindings_;
    std::map<std::string, std::shared_ptr<Command>> command_registry_;

public:
    InputHandler();
    void load_config(const std::string& filename);
    std::shared_ptr<Command> handle_input();
    std::string get_controls_help_string() const;

private:
    void register_commands();
    void set_default_controls();
    void save_config(const std::string& filename);
    char find_key_for_command(const std::string& cmd) const;
};

#endif