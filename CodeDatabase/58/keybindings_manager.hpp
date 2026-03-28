#pragma once

#include <filesystem>
#include <unordered_map>
#include <string>
#include "enum_command_type.hpp"
#include "json.hpp"

using json = nlohmann::json;

class IRenderer;

class KeybindingsManager{
private:
    std::filesystem::path config_dir = "config";
    std::filesystem::path config_file = config_dir / "keybindings.json";
    std::unordered_map<std::string, CommandType> keybindings;

    std::unordered_map<std::string, CommandType> get_default_bindings();
    bool validate_keybindings(std::unordered_map<std::string, CommandType>& bindings);
    CommandType string_to_command_type(std::string& str);
    std::string command_type_to_string(CommandType cmd);
    void save_keybindings_to_file();
public:
    KeybindingsManager();
    ~KeybindingsManager();

    std::unordered_map<std::string, CommandType> get_keybindings();

    bool config_file_exists();

    void setup_keybindings_interactive(IRenderer* renderer);
    void load_keybindings_from_file();
};
