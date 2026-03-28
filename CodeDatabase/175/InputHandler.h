#pragma once
#include "Command.h"
#include <memory>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>

class InputHandler {
public:
    virtual ~InputHandler() = default;
    virtual std::shared_ptr<Command> getNextCommand() = 0;
};

class KeyboardInputHandler : public InputHandler {
private:
    std::map<char, std::string> keyBindings;
    
    void loadKeyBindings(const std::string& configFile) {
        keyBindings.clear();
        std::ifstream file(configFile);
        if (file.is_open()) {
            std::cout << "Загружаем настройки управления из " << configFile << std::endl;
            
            std::string line;
            while (std::getline(file, line)) {
                if (line.empty() || line[0] == '#') continue;
                
                size_t eqPos = line.find('=');
                if (eqPos != std::string::npos) {
                    std::string keyStr = line.substr(0, eqPos);
                    std::string command = line.substr(eqPos + 1);
                    
                    keyStr.erase(std::remove_if(keyStr.begin(), keyStr.end(), ::isspace), keyStr.end());
                    command.erase(std::remove_if(command.begin(), command.end(), ::isspace), command.end());
                    
                    if (!keyStr.empty() && !command.empty()) {
                        char key = std::tolower(keyStr[0]);
                        keyBindings[key] = command;
                        std::cout << "Клавиша '" << key << "' -> команда '" << command << "'" << std::endl;
                    }
                }
            }
            file.close();
        }
        
        if (keyBindings.empty()) {
            std::cout << "Используются настройки управления по умолчанию" << std::endl;
            setDefaultBindings();
        }
    }
    
    void setDefaultBindings() {
        keyBindings = {
            {'w', "MOVE_UP"},
            {'s', "MOVE_DOWN"},
            {'a', "MOVE_LEFT"},
            {'d', "MOVE_RIGHT"},
            {'m', "SWITCH_ATTACK_MODE"},
            {'c', "USE_SPELL"},
            {'e', "END_TURN"},
            {'q', "SAVE_AND_EXIT"},
            {'l', "LOAD_GAME"}
        };
    }
    
    std::shared_ptr<Command> createCommand(const std::string& commandType) {
        if (commandType == "MOVE_UP") return std::make_shared<MoveCommand>(0, -1);
        if (commandType == "MOVE_DOWN") return std::make_shared<MoveCommand>(0, 1);
        if (commandType == "MOVE_LEFT") return std::make_shared<MoveCommand>(-1, 0);
        if (commandType == "MOVE_RIGHT") return std::make_shared<MoveCommand>(1, 0);
        if (commandType == "SWITCH_ATTACK_MODE") return std::make_shared<SwitchAttackModeCommand>();
        if (commandType == "USE_SPELL") return std::make_shared<UseSpellCommand>();
        if (commandType == "END_TURN") return std::make_shared<EndTurnCommand>();
        if (commandType == "SAVE_AND_EXIT") return std::make_shared<SaveCommand>("save.txt");
        if (commandType == "LOAD_GAME") return std::make_shared<LoadCommand>("save.txt");
        if (commandType == "EXIT") return std::make_shared<ExitCommand>();
        
        return nullptr;
    }
    
public:
    KeyboardInputHandler(const std::string& configFile = "controls.cfg") {
        loadKeyBindings(configFile);
    }
    
    std::shared_ptr<Command> getNextCommand() override {
        char input;
        std::cin >> input;
        input = std::tolower(input);
        
        auto it = keyBindings.find(input);
        if (it != keyBindings.end()) {
            return createCommand(it->second);
        }
        
        std::cout << "Клавиша '" << input << "' не назначена. Используй controls.cfg для настройки." << std::endl;
        return nullptr;
    }
    
    void printCurrentBindings() const {
        std::cout << "\n=== ТЕКУЩИЕ НАСТРОЙКИ УПРАВЛЕНИЯ ===" << std::endl;
        for (const auto& [key, cmd] : keyBindings) {
            std::cout << "'" << key << "' -> " << cmd << std::endl;
        }
    }
};