#include "key_config.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

KeyConfig::KeyConfig(const std::string& filename) {
    if (!Load(filename) || !Validate()) {
        std::cout << "⚠ Invalid key config. Using default mapping.\n";
        SetDefault();
    }
}

bool KeyConfig::Load(const std::string& filename)
{
    std::ifstream in(filename);
    if (!in) return false;
    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        std::string cmd, eq, key;
        iss >> cmd >> eq >> key; 
        if (eq != "=" || key.size() != 1)
            return false;
        char k = key[0];
        if (cmd == "quit") keyToCommand_[k] = CommandType::Quit;
        else if (cmd == "move_up") keyToCommand_[k] = CommandType::MoveUp;
        else if (cmd == "move_down") keyToCommand_[k] = CommandType::MoveDown;
        else if (cmd == "move_left") keyToCommand_[k] = CommandType::MoveLeft;
        else if (cmd == "move_right") keyToCommand_[k] = CommandType::MoveRight;
        else if (cmd == "toggle_mode") keyToCommand_[k] = CommandType::ToggleMode;
        else if (cmd == "use_spell") keyToCommand_[k] = CommandType::UseSpell;
        else if (cmd == "save") keyToCommand_[k] = CommandType::Save;
        else if (cmd == "load") keyToCommand_[k] = CommandType::Load;
        else return false;
    }
    return true;
}

bool KeyConfig::Validate() const
{
    if (keyToCommand_.size() != 9)
        return false;
    std::unordered_map<CommandType,int> count;
    for (auto& kv : keyToCommand_)
        count[kv.second]++;
    for (auto& c : count)
        if (c.second != 1)
            return false;
    return true;
}

void KeyConfig::SetDefault()
{
    keyToCommand_.clear();
    keyToCommand_['q'] = CommandType::Quit;
    keyToCommand_['w'] = CommandType::MoveUp;
    keyToCommand_['s'] = CommandType::MoveDown;
    keyToCommand_['a'] = CommandType::MoveLeft;
    keyToCommand_['d'] = CommandType::MoveRight;
    keyToCommand_['t'] = CommandType::ToggleMode;
    keyToCommand_['z'] = CommandType::UseSpell;
    keyToCommand_['p'] = CommandType::Save;
    keyToCommand_['l'] = CommandType::Load;
}

CommandType KeyConfig::Map(char c) const
{
    auto it = keyToCommand_.find(c);
    if (it == keyToCommand_.end())
        return CommandType::None;
    return it->second;
}
