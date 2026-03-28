#include "keybindings_manager.hpp"
#include "i_renderer.hpp"
#include <fstream>

KeybindingsManager::KeybindingsManager(){
    load_keybindings_from_file();
}

KeybindingsManager::~KeybindingsManager(){
}


std::unordered_map<std::string, CommandType> KeybindingsManager::get_default_bindings(){
    return {
        {"W", CommandType::MOVE_UP},
        {"A", CommandType::MOVE_LEFT},
        {"S", CommandType::MOVE_DOWN},
        {"D", CommandType::MOVE_RIGHT},
        {"CA", CommandType::CHANGE_ATTACK},
        {"CARDS", CommandType::SHOW_CARDS},
        {"USE", CommandType::USE_CARD},
        {"HELP", CommandType::HELP},
        {"END", CommandType::END_GAME},
        {"LOAD", CommandType::LOAD},
        {"SAVE", CommandType::SAVE},
        {"BIND", CommandType::KEYBIND_SETUP}
    };
}

CommandType KeybindingsManager::string_to_command_type(std::string& str){
    if (str == "MOVE_UP") return CommandType::MOVE_UP;
    if (str == "MOVE_LEFT") return CommandType::MOVE_LEFT;
    if (str == "MOVE_DOWN") return CommandType::MOVE_DOWN;
    if (str == "MOVE_RIGHT") return CommandType::MOVE_RIGHT;
    if (str == "CHANGE_ATTACK") return CommandType::CHANGE_ATTACK;
    if (str == "SHOW_CARDS") return CommandType::SHOW_CARDS;
    if (str == "USE_CARD") return CommandType::USE_CARD;
    if (str == "HELP") return CommandType::HELP;
    if (str == "END_GAME") return CommandType::END_GAME;
    if (str == "LOAD") return CommandType::LOAD;
    if (str == "SAVE") return CommandType::SAVE;
    if (str == "BIND") return CommandType::KEYBIND_SETUP;
    return CommandType::UNKNOWN;
}

std::string KeybindingsManager::command_type_to_string(CommandType cmd){
    switch (cmd){
        case CommandType::MOVE_UP: return "MOVE_UP";
        case CommandType::MOVE_LEFT: return "MOVE_LEFT";
        case CommandType::MOVE_DOWN: return "MOVE_DOWN";
        case CommandType::MOVE_RIGHT: return "MOVE_RIGHT";
        case CommandType::CHANGE_ATTACK: return "CHANGE_ATTACK";
        case CommandType::SHOW_CARDS: return "SHOW_CARDS";
        case CommandType::USE_CARD: return "USE_CARD";
        case CommandType::HELP: return "HELP";
        case CommandType::END_GAME: return "END_GAME";
        case CommandType::LOAD: return "LOAD";
        case CommandType::SAVE: return "SAVE";
        case CommandType::KEYBIND_SETUP: return "BIND";
        default: return "UNKNOWN";
    }
}

bool KeybindingsManager::validate_keybindings(std::unordered_map<std::string, CommandType>& bindings){
    std::vector<CommandType> required_cmd = {
        CommandType::MOVE_UP, CommandType::MOVE_LEFT, CommandType::MOVE_DOWN,
        CommandType::MOVE_RIGHT, CommandType::CHANGE_ATTACK, CommandType::SHOW_CARDS,
        CommandType::USE_CARD, CommandType::HELP, CommandType::END_GAME,
        CommandType::LOAD, CommandType::SAVE, CommandType::KEYBIND_SETUP
    };

    // check all cmd
    for (auto cmd : required_cmd){
        bool founded = false;
        for (auto& pair : bindings){
            if (pair.second == cmd){
                founded = true;
            }
        }
        if (!founded) return false;
    }

    // check duplicate keys
    std::unordered_map<std::string, int> key_count;
    for (auto& pair : bindings){
        std::string upper_key = pair.first;
        std::transform(upper_key.begin(), upper_key.end(), upper_key.begin(), ::toupper);

        key_count[upper_key]++;
        if (key_count[upper_key] > 1){
            return false;
        }
    }

    // check duplicate cmd
    std::unordered_map<CommandType, int> cmd_count;
    for (const auto& pair : bindings) {
        cmd_count[pair.second]++;
        if (cmd_count[pair.second] > 1) {
            return false;
        }
    }

    for (auto& pair : bindings){
        if (pair.second == CommandType::UNKNOWN){
            return false;
        }
    }
    return true;
}

void KeybindingsManager::load_keybindings_from_file(){
    if (!std::filesystem::exists(config_file)){
        keybindings = get_default_bindings();
        return;
    }

    std::ifstream file(config_file);
    if (!file.is_open()){
        std::cout << "warning: file is not open. using default keybindings" << std::endl;
        keybindings = get_default_bindings();
        return;
    }

    json j;
    try{
        file >> j;
    } catch (json::parse_error& e){
        std::cout << "warning: invalid json. using default keybindings" << std::endl;
        keybindings = get_default_bindings();
        return;
    }

    std::unordered_map<std::string, CommandType> loaded_bindings;
    for (auto it = j.begin(); it != j.end(); it++){
        if(!it.value().is_string()){
            std::cout << "warning: invalid json. using default keybindings" << std::endl;
            keybindings = get_default_bindings();
            return;
        }

        std::string key = it.key();
        std::string cmd_str = it.value();
        std::transform(key.begin(), key.end(), key.begin(), ::toupper);

        CommandType cmd = string_to_command_type(cmd_str);
        if (cmd != CommandType::UNKNOWN){
            loaded_bindings[key] = cmd;
        }
    }

    if (validate_keybindings(loaded_bindings)){
        keybindings = loaded_bindings;
    } else {
        std::cout << "warning: invalid keybindings configurations. using default keybindings" << std::endl;
        keybindings = get_default_bindings();
    }
}

void KeybindingsManager::save_keybindings_to_file(){
    if (!std::filesystem::exists(config_dir)) {
        std::filesystem::create_directory(config_dir);
    }

    json j;
    for (auto& pair : keybindings){
        j[pair.first] = command_type_to_string(pair.second);
    }

    std::ofstream file(config_file);
    if (!file.is_open()){
        std::cout << "error: cannot open config file for writing" << std::endl;
        return;
    }

    file << j.dump(4);
    file.close();
}

std::unordered_map<std::string, CommandType> KeybindingsManager::get_keybindings() {
    return keybindings;
}

bool KeybindingsManager::config_file_exists() {
    return std::filesystem::exists(config_file);
}

void KeybindingsManager::setup_keybindings_interactive(IRenderer* renderer){
    if (!renderer){
        std::cout << "error: renderer is null" << std::endl;
        return; 
    }

    std::vector<CommandType> commands = {
        CommandType::MOVE_UP, CommandType::MOVE_LEFT, CommandType::MOVE_DOWN,
        CommandType::MOVE_RIGHT, CommandType::CHANGE_ATTACK, CommandType::SHOW_CARDS,
        CommandType::USE_CARD, CommandType::HELP, CommandType::END_GAME,
        CommandType::LOAD, CommandType::SAVE, CommandType::KEYBIND_SETUP
    };
    std::unordered_map<std::string, CommandType> new_bindings;

    std::unordered_map<CommandType, std::string> current_bindings;
    for (auto& pair : keybindings) {
        current_bindings[pair.second] = pair.first;
    }

    renderer->render_msg("=== Keybindings Setup ===");
    renderer->render_msg("");

    for (auto cmd : commands){
        std::string cmd_name = command_type_to_string(cmd);
        std::string current_key = current_bindings.count(cmd) > 0 ? current_bindings[cmd] : "none";

        std::string input;
        std::cout << cmd_name << " (current: " << current_key << "): ";
        std::getline(std::cin, input);

        if(input.empty()){
            if (current_bindings.count(cmd) > 0){
                new_bindings[current_bindings[cmd]] = cmd;
            } else{
                auto defaults = get_default_bindings();
                for (auto& pair : defaults){
                    if (pair.second == cmd){
                        new_bindings[pair.first] = cmd;
                        break;
                    }
                }
            }
            continue;
        }

        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        bool key_already_used = false;
        for (auto& pair : new_bindings){
            if (pair.first == input && pair.second != cmd){
                std::cout << "warning: key '" << input << "' is already used to "
                << command_type_to_string(pair.second) << std::endl;
                key_already_used = true;
                break;
            }
        }
        if (!key_already_used){
            new_bindings[input] = cmd;
        } else{
            auto defaults = get_default_bindings();
            for (auto& pair : defaults){
                if (pair.second == cmd){
                    new_bindings[pair.first] = cmd;
                    break;
                }
            }
        }
    }

    if (!validate_keybindings(new_bindings)){
        std::cout << "error: invalid keybindings cinfigurations. using default" << std::endl;
        keybindings = get_default_bindings();
    } else {
        keybindings = new_bindings;
        save_keybindings_to_file();
        renderer->render_msg("keybindings saved!");
    }
}