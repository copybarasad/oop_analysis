#pragma once

#include <map>
#include <string>
#include <optional>
#include <vector>
#include "Command.h"

class KeyBindings {
public:
    KeyBindings();

    void LoadFromFile(const std::string& filename);

    CommandType GetCommandForKey(char c) const;

private:
    std::map<char, CommandType> key_to_command_;

    void SetDefault();
    static std::optional<CommandType> ParseCommand(const std::string& s_raw);
    static std::vector<CommandType> AllRequiredCommands();
};
