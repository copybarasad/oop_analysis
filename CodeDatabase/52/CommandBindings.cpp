#include "CommandBindings.h"
#include <fstream>
#include <set>

CommandBindings::CommandBindings() {
    SetDefault();
}

void CommandBindings::SetDefault() {
    keyToCmd_.clear();
    keyToCmd_['W'] = Command::MoveUp;
    keyToCmd_['S'] = Command::MoveDown;
    keyToCmd_['A'] = Command::MoveLeft;
    keyToCmd_['D'] = Command::MoveRight;
    keyToCmd_['F'] = Command::Attack;
    keyToCmd_['C'] = Command::CastSpell;
    keyToCmd_['R'] = Command::SwitchMode;
    keyToCmd_['L'] = Command::Save;
    keyToCmd_['Q'] = Command::Quit;
}

bool CommandBindings::LoadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        SetDefault();
        return false;
    }

    keyToCmd_.clear();

    std::unordered_map<std::string, Command> nameMap = {
        {"MoveUp", Command::MoveUp},
        {"MoveDown", Command::MoveDown},
        {"MoveLeft", Command::MoveLeft},
        {"MoveRight", Command::MoveRight},
        {"Attack", Command::Attack},
        {"CastSpell", Command::CastSpell},
        {"SwitchMode", Command::SwitchMode},
        {"Save", Command::Save},
        {"Quit", Command::Quit}
    };

    std::string line;
    while (std::getline(in, line)) {
        auto pos = line.find('=');
        if (pos == std::string::npos) {
            SetDefault();
            return false;
        }

        std::string name = line.substr(0, pos);
        char key = std::toupper(line[pos + 1]);

        if (!nameMap.count(name)) {
            SetDefault();
            return false;
        }

        keyToCmd_[key] = nameMap[name];
    }

    if (!Validate()) {
        SetDefault();
        return false;
    }

    return true;
}

bool CommandBindings::Validate() const {
    std::set<Command> cmds;
    for (auto& [_, cmd] : keyToCmd_)
        cmds.insert(cmd);

    if (cmds.size() != 9) // количество команд
        return false;

    std::set<char> keys;
    for (auto& [k, _] : keyToCmd_) {
        if (!keys.insert(k).second)
            return false;
    }

    return true;
}

Command CommandBindings::GetCommand(char key) const noexcept {
    key = std::toupper(key);
    auto it = keyToCmd_.find(key);
    return (it != keyToCmd_.end()) ? it->second : Command::None;
}
    

std::vector<std::pair<Command, char>> CommandBindings::GetAllBindings() const {
    std::vector<std::pair<Command, char>> res;
    for (auto& [key, cmd] : keyToCmd_) {
        res.emplace_back(cmd, key);
    }
    return res;
}