#ifndef INPUT_CONFIG_H
#define INPUT_CONFIG_H

#include "Command.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

class InputConfig {
public:
    InputConfig();
    bool loadFromFile(const std::string& path);
    CommandType commandForKey(char key) const;
    bool isValid() const;
    std::unordered_map<CommandType, char> getBindings() const;

private:
    void setDefault();
    bool validate() const;

private:
    std::unordered_map<char, CommandType> keyToCommand_;
    bool valid_;
};

#endif

