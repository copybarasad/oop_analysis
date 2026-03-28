#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "Architecture.h"
#include "json.hpp" // nlohmann/json

using json = nlohmann::json;

class InputConfig {
public:
    static std::map<sf::Keyboard::Key, CommandType> loadBindings(const std::string& filename) {
        // Если файла .json нет, пробуем загрузить дефолт (или старый cfg, если нужно)
        std::ifstream file(filename);
        if (!file.is_open()) {
            // Если ищем .cfg, а перешли на .json, меняем расширение
            std::string jsonName = "keys.json";
            std::ifstream jsonFile(jsonName);
            if (jsonFile.is_open()) {
                return parseJson(jsonFile);
            }
            std::cerr << "Config file not found. Loading defaults.\n";
            return getDefaultBindings();
        }
        
        // Если открыли файл - парсим
        return parseJson(file);
    }

private:
    static std::map<sf::Keyboard::Key, CommandType> parseJson(std::ifstream& file) {
        std::map<sf::Keyboard::Key, CommandType> bindings;
        json j;
        try {
            file >> j;
            for (auto& element : j.items()) {
                std::string cmdStr = element.key();
                std::string keyStr = element.value();

                CommandType cmd = stringToCommand(cmdStr);
                sf::Keyboard::Key key = stringToKey(keyStr);

                if (cmd != CommandType::None && key != sf::Keyboard::Key::Unknown) {
                    bindings[key] = cmd;
                }
            }
            std::cout << "JSON controls loaded successfully.\n";
        } catch (const std::exception& e) {
            std::cerr << "JSON Parse Error: " << e.what() << "\n";
            return getDefaultBindings();
        }
        return bindings;
    }

public:
    static std::map<sf::Keyboard::Key, CommandType> getDefaultBindings() {
        return {
            {sf::Keyboard::Key::W, CommandType::MoveUp},
            {sf::Keyboard::Key::S, CommandType::MoveDown},
            {sf::Keyboard::Key::A, CommandType::MoveLeft},
            {sf::Keyboard::Key::D, CommandType::MoveRight},
            {sf::Keyboard::Key::Space, CommandType::Attack},
            {sf::Keyboard::Key::Num1, CommandType::CastSpell},
            {sf::Keyboard::Key::Num2, CommandType::NextSpell},
            {sf::Keyboard::Key::Num3, CommandType::PrevSpell},
            {sf::Keyboard::Key::E, CommandType::SwitchMode},
            {sf::Keyboard::Key::P, CommandType::Pause},
            {sf::Keyboard::Key::F5, CommandType::Save},
            {sf::Keyboard::Key::F9, CommandType::Load},
            {sf::Keyboard::Key::Q, CommandType::Quit},
            {sf::Keyboard::Key::Escape, CommandType::Exit},
            {sf::Keyboard::Key::Up, CommandType::TargetMoveUp},
            {sf::Keyboard::Key::Down, CommandType::TargetMoveDown},
            {sf::Keyboard::Key::Left, CommandType::TargetMoveLeft},
            {sf::Keyboard::Key::Right, CommandType::TargetMoveRight},
            {sf::Keyboard::Key::Enter, CommandType::TargetSelect}
        };
    }

private:
    static CommandType stringToCommand(const std::string& s) {
        if (s == "MoveUp") return CommandType::MoveUp;
        if (s == "MoveDown") return CommandType::MoveDown;
        if (s == "MoveLeft") return CommandType::MoveLeft;
        if (s == "MoveRight") return CommandType::MoveRight;
        if (s == "Attack") return CommandType::Attack;
        if (s == "SwitchMode") return CommandType::SwitchMode;
        if (s == "CastSpell") return CommandType::CastSpell;
        if (s == "NextSpell") return CommandType::NextSpell;
        if (s == "PrevSpell") return CommandType::PrevSpell;
        if (s == "Pause") return CommandType::Pause;
        if (s == "Save") return CommandType::Save;
        if (s == "Load") return CommandType::Load;
        if (s == "Quit") return CommandType::Quit;
        if (s == "Exit") return CommandType::Exit;
        if (s == "TargetUp") return CommandType::TargetMoveUp;
        if (s == "TargetDown") return CommandType::TargetMoveDown;
        if (s == "TargetLeft") return CommandType::TargetMoveLeft;
        if (s == "TargetRight") return CommandType::TargetMoveRight;
        if (s == "TargetSelect") return CommandType::TargetSelect;
        return CommandType::None;
    }

    static sf::Keyboard::Key stringToKey(const std::string& s) {
        if (s.length() == 1) {
            char c = std::toupper(s[0]);
            if (c >= 'A' && c <= 'Z') return static_cast<sf::Keyboard::Key>(static_cast<int>(sf::Keyboard::Key::A) + (c - 'A'));
            if (c >= '0' && c <= '9') return static_cast<sf::Keyboard::Key>(static_cast<int>(sf::Keyboard::Key::Num0) + (c - '0'));
        }
        if (s == "F1") return sf::Keyboard::Key::F1;
        if (s == "F2") return sf::Keyboard::Key::F2;
        if (s == "F3") return sf::Keyboard::Key::F3;
        if (s == "F4") return sf::Keyboard::Key::F4;
        if (s == "F5") return sf::Keyboard::Key::F5;
        if (s == "F6") return sf::Keyboard::Key::F6;
        if (s == "F7") return sf::Keyboard::Key::F7;
        if (s == "F8") return sf::Keyboard::Key::F8;
        if (s == "F9") return sf::Keyboard::Key::F9;
        if (s == "F10") return sf::Keyboard::Key::F10;
        if (s == "F11") return sf::Keyboard::Key::F11;
        if (s == "F12") return sf::Keyboard::Key::F12;
        
        if (s == "Num0") return sf::Keyboard::Key::Num0;
        if (s == "Num1") return sf::Keyboard::Key::Num1;
        if (s == "Num2") return sf::Keyboard::Key::Num2;
        if (s == "Num3") return sf::Keyboard::Key::Num3;
        if (s == "Num4") return sf::Keyboard::Key::Num4;
        if (s == "Num5") return sf::Keyboard::Key::Num5;
        if (s == "Num6") return sf::Keyboard::Key::Num6;
        if (s == "Num7") return sf::Keyboard::Key::Num7;
        if (s == "Num8") return sf::Keyboard::Key::Num8;
        if (s == "Num9") return sf::Keyboard::Key::Num9;
        if (s == "Esc" || s == "Escape") return sf::Keyboard::Key::Escape;
        if (s == "Enter") return sf::Keyboard::Key::Enter;
        if (s == "Space") return sf::Keyboard::Key::Space;
        if (s == "Tab") return sf::Keyboard::Key::Tab;
        if (s == "Up") return sf::Keyboard::Key::Up;
        if (s == "Down") return sf::Keyboard::Key::Down;
        if (s == "Left") return sf::Keyboard::Key::Left;
        if (s == "Right") return sf::Keyboard::Key::Right;
        if (s == "LShift") return sf::Keyboard::Key::LShift;
        if (s == "RShift") return sf::Keyboard::Key::RShift;
        if (s == "LCtrl") return sf::Keyboard::Key::LControl;
        if (s == "RCtrl") return sf::Keyboard::Key::RControl;
        return sf::Keyboard::Key::Unknown;
    }
};
