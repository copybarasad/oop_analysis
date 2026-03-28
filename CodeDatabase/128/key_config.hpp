#pragma once
#include <unordered_map>
#include <string>
#include "enums.hpp"

class KeyConfig {
public:
    KeyConfig(const std::string& filename);

    CommandType Map(char c) const;

private:
    bool Load(const std::string& filename);
    bool Validate() const;
    void SetDefault();

    std::unordered_map<char, CommandType> keyToCommand_;
};
