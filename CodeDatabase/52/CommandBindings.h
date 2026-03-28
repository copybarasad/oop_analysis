#pragma once
#include <vector>     
#include <utility>
#include <unordered_map>
#include <string>
#include "Command.h"


class CommandBindings {
public:
    CommandBindings();

    bool LoadFromFile(const std::string& filename);

    Command GetCommand(char key) const noexcept;
    std::vector<std::pair<Command, char>> GetAllBindings() const;

private:
    std::unordered_map<char, Command> keyToCmd_;

    void SetDefault();
    bool Validate() const;
    
};
