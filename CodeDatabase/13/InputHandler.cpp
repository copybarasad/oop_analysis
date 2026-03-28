#include "InputHandler.h"
#include <SFML/Window/Keyboard.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using nlohmann::json;

InputHandler::InputHandler() {
    loadBindings();
}

void InputHandler::loadBindings() {
    std::map<sf::Keyboard::Key, CommandType> loaded;
    if (!loadFromFile(loaded)) {
        std::cerr << "Controls config invalid or missing, using default bindings\n";
        setDefaultBindings();
    } else {
        bindings_ = std::move(loaded);
    }
}

void InputHandler::setDefaultBindings() {
    bindings_.clear();

    json j;
    j["Exit"] = "Escape";
    j["SaveGame"] = "F1";
    j["LoadGame"] = "F2";

    j["MoveUp"] = "W";
    j["MoveDown"] = "S";
    j["MoveLeft"] = "A";
    j["MoveRight"] = "D";

    j["ArrowLeft"] = "Left";
    j["ArrowRight"] = "Right";
    j["ArrowUp"] = "Up";
    j["ArrowDown"] = "Down";

    j["ToggleBook"] = "Q";
    j["ToggleSpellInfo"] = "I";
    j["CastOrConfirm"] = "Space";
    j["ChangeWeapon"] = "E";

    std::ofstream outFile("controls.json");
    if (outFile.is_open()) {
        outFile << j.dump(4);
        outFile.close();
        std::cout << "Created new controls.json with default bindings\n";
    } else {
        std::cerr << "Failed to create controls.json, using in-code defaults\n";
    }

    bindings_[sf::Keyboard::Escape] = CommandType::Exit;
    bindings_[sf::Keyboard::F1] = CommandType::SaveGame;
    bindings_[sf::Keyboard::F2] = CommandType::LoadGame;

    bindings_[sf::Keyboard::W] = CommandType::MoveUp;
    bindings_[sf::Keyboard::S] = CommandType::MoveDown;
    bindings_[sf::Keyboard::A] = CommandType::MoveLeft;
    bindings_[sf::Keyboard::D] = CommandType::MoveRight;

    bindings_[sf::Keyboard::Left] = CommandType::ArrowLeft;
    bindings_[sf::Keyboard::Right] = CommandType::ArrowRight;
    bindings_[sf::Keyboard::Up] = CommandType::ArrowUp;
    bindings_[sf::Keyboard::Down] = CommandType::ArrowDown;

    bindings_[sf::Keyboard::Q] = CommandType::ToggleBook;
    bindings_[sf::Keyboard::I] = CommandType::ToggleSpellInfo;
    bindings_[sf::Keyboard::Space] = CommandType::CastOrConfirm;
    bindings_[sf::Keyboard::E] = CommandType::ChangeWeapon;
}

bool InputHandler::loadFromFile(std::map<sf::Keyboard::Key, CommandType> &out) {
    std::ifstream in("controls.json");
    if (!in.is_open()) {
        return false;
    }

    json j;
    try {
        in >> j;
    } catch (...) {
        return false;
    }

    const std::vector<std::string> requiredCommands = {
        "Exit",
        "SaveGame",
        "LoadGame",
        "MoveUp",
        "MoveDown",
        "MoveLeft",
        "MoveRight",
        "ArrowLeft",
        "ArrowRight",
        "ArrowUp",
        "ArrowDown",
        "ToggleBook",
        "ToggleSpellInfo",
        "CastOrConfirm",
        "ChangeWeapon"
    };

    std::map<sf::Keyboard::Key, CommandType> keyToCmd;
    std::map<CommandType, sf::Keyboard::Key> cmdToKey;

    try {
        CommandType cmd;
        for (const std::string &name: requiredCommands) {
            if (!j.contains(name)) {
                throw std::runtime_error("Missing command: " + name);
            }

            if (!j[name].is_string()) {
                throw std::runtime_error("Key for command is not string: " + name);
            }

            std::string keyName = j[name].get<std::string>();

            sf::Keyboard::Key key;
            if (!stringToKey(keyName, key)) {
                throw std::runtime_error("Unknown key: " + keyName);
            }
            
            stringToCommand(name, cmd);
            auto itKey = keyToCmd.find(key);
            if (itKey != keyToCmd.end() && itKey->second != cmd) {
                throw std::runtime_error("Two commands on one key");
            }

            auto itCmd = cmdToKey.find(cmd);
            if (itCmd != cmdToKey.end() && itCmd->second != key) {
                throw std::runtime_error("One command has two keys");
            }

            keyToCmd[key] = cmd;
            cmdToKey[cmd] = key;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return false;
    }

    out = std::move(keyToCmd);
    return true;
}

CommandType InputHandler::parseEvent(const sf::Event &event) {
    if (event.type == sf::Event::Closed) {
        return CommandType::CloseGame;
    }

    if (event.type != sf::Event::KeyPressed) {
        return CommandType::None;
    }

    auto it = bindings_.find(event.key.code);
    if (it != bindings_.end()) {
        return it->second;
    }

    return CommandType::None;
}

bool InputHandler::stringToKey(const std::string &name, sf::Keyboard::Key &key) {
    using K = sf::Keyboard::Key;

    std::string s = name;
    std::ranges::transform(s, s.begin(),
                           [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

    if (s.size() == 1) {
        char c = s[0];

        if (c >= 'A' && c <= 'Z') {
            key = static_cast<K>(K::A + (c - 'A'));
            return true;
        }
        if (c >= '0' && c <= '9') {
            key = static_cast<K>(K::Num0 + (c - '0'));
            return true;
        }
    }

    static const std::map<std::string, K> namedKeys = {
        {"ESCAPE", K::Escape},
        {"SPACE", K::Space},
        {"ENTER", K::Enter},
        {"TAB", K::Tab},

        {"LEFT", K::Left},
        {"RIGHT", K::Right},
        {"UP", K::Up},
        {"DOWN", K::Down},

        {"NUM0", K::Num0},
        {"NUM1", K::Num1},
        {"NUM2", K::Num2},
        {"NUM3", K::Num3},
        {"NUM4", K::Num4},
        {"NUM5", K::Num5},
        {"NUM6", K::Num6},
        {"NUM7", K::Num7},
        {"NUM8", K::Num8},
        {"NUM9", K::Num9},

        {"F1", K::F1},
        {"F2", K::F2},
        {"F3", K::F3},
        {"F4", K::F4},
        {"F5", K::F5},
        {"F6", K::F6},
        {"F7", K::F7},
        {"F8", K::F8},
        {"F9", K::F9},
        {"F10", K::F10},
        {"F11", K::F11},
        {"F12", K::F12},
        {"F13", K::F13},
        {"F14", K::F14},
        {"F15", K::F15},

        {"LSHIFT", K::LShift},
        {"RSHIFT", K::RShift},
        {"LCONTROL", K::LControl},
        {"RCONTROL", K::RControl},
    };

    auto it = namedKeys.find(s);
    if (it != namedKeys.end()) {
        key = it->second;
        return true;
    }

    return false;
}

bool InputHandler::stringToCommand(const std::string &name, CommandType &cmd) {
    static const std::map<std::string, CommandType> cmdMap = {
        {"Exit", CommandType::Exit},
        {"SaveGame", CommandType::SaveGame},
        {"LoadGame", CommandType::LoadGame},
        {"MoveUp", CommandType::MoveUp},
        {"MoveDown", CommandType::MoveDown},
        {"MoveLeft", CommandType::MoveLeft},
        {"MoveRight", CommandType::MoveRight},
        {"ArrowLeft", CommandType::ArrowLeft},
        {"ArrowRight", CommandType::ArrowRight},
        {"ArrowUp", CommandType::ArrowUp},
        {"ArrowDown", CommandType::ArrowDown},
        {"ToggleBook", CommandType::ToggleBook},
        {"ToggleSpellInfo", CommandType::ToggleSpellInfo},
        {"CastOrConfirm", CommandType::CastOrConfirm},
        {"ChangeWeapon", CommandType::ChangeWeapon}
    };

    auto it = cmdMap.find(name);
    if (it == cmdMap.end()) {
        return false;
    }
    cmd = it->second;
    return true;
}
