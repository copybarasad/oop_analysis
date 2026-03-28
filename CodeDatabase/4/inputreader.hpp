#pragma once
#include "command.hpp"
#include <unordered_map>
#include <string>

class InputReader {
protected:
    std::unordered_map<char, GameAction> keymap;

    void load_default_mapping();
    bool is_valid_mapping() const;

public:
    InputReader(const std::string& config_path = "control.cfg");
    virtual ~InputReader() = default;
    virtual Command read_command() = 0;
};