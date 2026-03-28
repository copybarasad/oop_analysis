#pragma once

#include "Command.h"
#include <functional>
#include <unordered_map>
#include <string>

class ControlMapping {
public:
    ControlMapping();

    bool loadFromFile(const std::string &path,
                      std::function<void(const std::string &)> reporter = {});

    CommandType commandForKey(char key) const;

    char keyFor(CommandType cmd) const;

    const std::unordered_map<char, CommandType> &bindings() const;

private:
    std::unordered_map<char, CommandType> keyToCommand;

    void setDefault();

    bool isValid(const std::unordered_map<char, CommandType> &candidate) const;
};
