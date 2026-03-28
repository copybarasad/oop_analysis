#pragma once
#include <map>
#include <string>
#include <functional>

class Command;

class InputReader {
private:
    std::map<char, std::function<Command* ()>> key_map;

    void load_config(const std::string& filename);
    void set_defaults();

public:
    InputReader(const std::string& config_file = "controls.txt");
    Command* get_command();
};
