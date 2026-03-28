#ifndef KEY_BINDINGS_H
#define KEY_BINDINGS_H

#include <map>
#include <string>
#include <set>
#include "command_type.h"

class KeyBindings
{
private:
    std::map<char, CommandType> key_to_command;
    std::map<char, int> move_directions_dx;
    std::map<char, int> move_directions_dy;

    void set_default_bindings();
    bool validate() const;
    bool is_digit_key(char key) const;

public:
    KeyBindings();

    void set_binding(char key, CommandType type);
    void set_move_binding(char key, int dx, int dy);
    
    CommandType get_command_type(char key) const;
    char get_key_for_command(CommandType type) const;
    char get_move_key(int dx, int dy) const;
    bool has_binding(char key) const;
    int get_move_dx(char key) const;
    int get_move_dy(char key) const;
    
    std::string get_controls_help() const;

    bool load_from_file(const std::string& filepath);
    void reset_to_default();

    static const std::string DEFAULT_CONFIG_PATH;
};

#endif