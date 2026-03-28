#include "key_bindings.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

const std::string KeyBindings::DEFAULT_CONFIG_PATH = "config/keybindings.txt";

KeyBindings::KeyBindings()
{
    set_default_bindings();
}

void KeyBindings::set_default_bindings()
{
    key_to_command.clear();
    move_directions_dx.clear();
    move_directions_dy.clear();

    key_to_command['w'] = CommandType::MOVE;
    key_to_command['a'] = CommandType::MOVE;
    key_to_command['s'] = CommandType::MOVE;
    key_to_command['d'] = CommandType::MOVE;

    move_directions_dx['w'] = 0;  move_directions_dy['w'] = -1;
    move_directions_dx['a'] = -1; move_directions_dy['a'] = 0;
    move_directions_dx['s'] = 0;  move_directions_dy['s'] = 1;
    move_directions_dx['d'] = 1;  move_directions_dy['d'] = 0;

    key_to_command['q'] = CommandType::SWITCH_MODE;
    key_to_command['e'] = CommandType::ATTACK;
    key_to_command['f'] = CommandType::SAVE;
    key_to_command['l'] = CommandType::LOAD;
    key_to_command['x'] = CommandType::EXIT;
}

void KeyBindings::reset_to_default()
{
    set_default_bindings();
}

void KeyBindings::set_binding(char key, CommandType type)
{
    key_to_command[key] = type;
}

void KeyBindings::set_move_binding(char key, int dx, int dy)
{
    key_to_command[key] = CommandType::MOVE;
    move_directions_dx[key] = dx;
    move_directions_dy[key] = dy;
}

CommandType KeyBindings::get_command_type(char key) const
{
    auto it = key_to_command.find(key);
    if (it != key_to_command.end())
    {
        return it->second;
    }
    return CommandType::INVALID;
}

bool KeyBindings::has_binding(char key) const
{
    return key_to_command.find(key) != key_to_command.end();
}

int KeyBindings::get_move_dx(char key) const
{
    auto it = move_directions_dx.find(key);
    return (it != move_directions_dx.end()) ? it->second : 0;
}

int KeyBindings::get_move_dy(char key) const
{
    auto it = move_directions_dy.find(key);
    return (it != move_directions_dy.end()) ? it->second : 0;
}

bool KeyBindings::is_digit_key(char key) const
{
    return key >= '1' && key <= '9';
}

bool KeyBindings::validate() const
{
    std::set<CommandType> required =
    {
        CommandType::ATTACK,
        CommandType::SWITCH_MODE,
        CommandType::SAVE,
        CommandType::LOAD,
        CommandType::EXIT
    };

    int move_up = 0, move_down = 0, move_left = 0, move_right = 0;

    std::map<CommandType, std::set<char>> command_to_keys;

    for (const auto& pair : key_to_command)
    {
        char key = pair.first;
        CommandType cmd = pair.second;

        if (is_digit_key(key))
        {
            return false;
        }

        command_to_keys[cmd].insert(key);

        if (cmd == CommandType::MOVE)
        {
            int dx = get_move_dx(key);
            int dy = get_move_dy(key);
            
            if (dx == 0 && dy == -1) move_up++;
            else if (dx == 0 && dy == 1) move_down++;
            else if (dx == -1 && dy == 0) move_left++;
            else if (dx == 1 && dy == 0) move_right++;
        }
    }

    if (move_up != 1 || move_down != 1 || move_left != 1 || move_right != 1)
    {
        return false;
    }

    for (CommandType cmd : required)
    {
        auto it = command_to_keys.find(cmd);
        if (it == command_to_keys.end() || it->second.size() != 1)
        {
            return false;
        }
    }

    return true;
}

bool KeyBindings::load_from_file(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        return false;
    }

    KeyBindings temp_bindings;
    temp_bindings.key_to_command.clear();
    temp_bindings.move_directions_dx.clear();
    temp_bindings.move_directions_dy.clear();

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        std::istringstream iss(line);
        std::string command_name;
        std::string key_str;

        if (!(iss >> command_name >> key_str))
        {
            continue;
        }

        char key;
        if (key_str == "space")
        {
            key = ' ';
        }
        else
        {
            key = std::tolower(key_str[0]);
        }

        if (command_name == "move_up")
        {
            temp_bindings.set_move_binding(key, 0, -1);
        }
        else if (command_name == "move_down")
        {
            temp_bindings.set_move_binding(key, 0, 1);
        }
        else if (command_name == "move_left")
        {
            temp_bindings.set_move_binding(key, -1, 0);
        }
        else if (command_name == "move_right")
        {
            temp_bindings.set_move_binding(key, 1, 0);
        }
        else if (command_name == "attack")
        {
            temp_bindings.set_binding(key, CommandType::ATTACK);
        }
        else if (command_name == "switch_mode")
        {
            temp_bindings.set_binding(key, CommandType::SWITCH_MODE);
        }
        else if (command_name == "save")
        {
            temp_bindings.set_binding(key, CommandType::SAVE);
        }
        else if (command_name == "load")
        {
            temp_bindings.set_binding(key, CommandType::LOAD);
        }
        else if (command_name == "exit")
        {
            temp_bindings.set_binding(key, CommandType::EXIT);
        }
    }

    file.close();

    if (!temp_bindings.validate())
    {
        return false;
    }

    key_to_command = std::move(temp_bindings.key_to_command);
    move_directions_dx = std::move(temp_bindings.move_directions_dx);
    move_directions_dy = std::move(temp_bindings.move_directions_dy);

    return true;
}

char KeyBindings::get_key_for_command(CommandType type) const
{
    for (const auto& pair : key_to_command)
    {
        if (pair.second == type)
        {
            return pair.first;
        }
    }
    return '?';
}

char KeyBindings::get_move_key(int dx, int dy) const
{
    for (const auto& pair : key_to_command)
    {
        if (pair.second == CommandType::MOVE)
        {
            if (get_move_dx(pair.first) == dx && get_move_dy(pair.first) == dy)
            {
                return pair.first;
            }
        }
    }
    return '?';
}

std::string KeyBindings::get_controls_help() const
{
    std::ostringstream oss;
    
    char up = std::toupper(get_move_key(0, -1));
    char down = std::toupper(get_move_key(0, 1));
    char left = std::toupper(get_move_key(-1, 0));
    char right = std::toupper(get_move_key(1, 0));
    
    oss << up << left << down << right << " to move, ";
    oss << static_cast<char>(std::toupper(get_key_for_command(CommandType::SWITCH_MODE))) << " to switch mode, ";
    oss << static_cast<char>(std::toupper(get_key_for_command(CommandType::ATTACK))) << " to attack, ";
    oss << static_cast<char>(std::toupper(get_key_for_command(CommandType::SAVE))) << " to save, ";
    oss << static_cast<char>(std::toupper(get_key_for_command(CommandType::LOAD))) << " to load, ";
    oss << static_cast<char>(std::toupper(get_key_for_command(CommandType::EXIT))) << " to exit";
    
    return oss.str();
}