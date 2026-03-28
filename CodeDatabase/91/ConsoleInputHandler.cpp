#include "ConsoleInputHandler.h"
#include "../Commands/MoveUpCommand.h"
#include "../Commands/MoveDownCommand.h"
#include "../Commands/MoveLeftCommand.h"
#include "../Commands/MoveRightCommand.h"
#include "../Commands/SwitchCombatCommand.h"
#include "../Commands/RangedAttackCommand.h"
#include "../Commands/CastSpellCommand.h"
#include "../Commands/BuySpellCommand.h"
#include "../Commands/SaveGameCommand.h"
#include "../Commands/QuitGameCommand.h"

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <set>
#include "../json.hpp"

ConsoleInputHandler::ConsoleInputHandler(const std::string& configPath) {
    if (!configPath.empty()) {
        loadControls(configPath);
    }

    // default
    if (keyMap.empty()) {
        std::cout << "Using default controls." << std::endl;
        keyMap['W'] = CommandType::MOVE_UP;
        keyMap['S'] = CommandType::MOVE_DOWN;
        keyMap['A'] = CommandType::MOVE_LEFT;
        keyMap['D'] = CommandType::MOVE_RIGHT;
        keyMap['C'] = CommandType::SWITCH_COMBAT;
        keyMap['F'] = CommandType::RANGED_ATTACK;
        keyMap['M'] = CommandType::CAST_SPELL;
        keyMap['B'] = CommandType::BUY_SPELL;
        keyMap['X'] = CommandType::SAVE_GAME;
        keyMap['Q'] = CommandType::QUIT_GAME;
    }
}

std::unique_ptr<ICommand> ConsoleInputHandler::getCommand() {
    char input = '\0';
    std::string inputLine;
    std::getline(std::cin, inputLine);

    if (inputLine.length() != 1) {
        std::cout << "Please enter only ONE character!" << std::endl;
        return nullptr;
    }

    if (!inputLine.empty()) {
        input = std::toupper(inputLine[0]);
    } else {
        return nullptr;
    }

    auto it = keyMap.find(input);
    if (it != keyMap.end()) {
        switch (it->second) {
            case CommandType::MOVE_UP:
                return std::make_unique<MoveUpCommand>();
            case CommandType::MOVE_DOWN:
                return std::make_unique<MoveDownCommand>();
            case CommandType::MOVE_LEFT:
                return std::make_unique<MoveLeftCommand>();
            case CommandType::MOVE_RIGHT:
                return std::make_unique<MoveRightCommand>();
            case CommandType::SWITCH_COMBAT:
                return std::make_unique<SwitchCombatCommand>();
            case CommandType::RANGED_ATTACK:
                return std::make_unique<RangedAttackCommand>();
            case CommandType::CAST_SPELL:
                return std::make_unique<CastSpellCommand>();
            case CommandType::BUY_SPELL:
                return std::make_unique<BuySpellCommand>();
            case CommandType::SAVE_GAME:
                return std::make_unique<SaveGameCommand>();
            case CommandType::QUIT_GAME:
                return std::make_unique<QuitGameCommand>();
            default:
                std::cout << "Warning: Unknown command type for key '" << input << "'." << std::endl;
                return nullptr;
        }
    } else {
        std::cout << "Invalid command: '" << input << "'. Please try again." << std::endl;
        return nullptr;
    }
}

void ConsoleInputHandler::loadControls(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cout << "Warning: Could not open control config file: " << filepath << std::endl;
        return;
    }

    try {
        nlohmann::json j;
        file >> j;
        file.close();

        std::map<char, CommandType> newMap;
        bool isValid = true;

        for (const auto& item : j.items()) {
            const std::string& cmd_str = item.key();
            const nlohmann::json& key_val = item.value();

            if (!key_val.is_string() || key_val.get<std::string>().length() != 1) {
                std::cout << "Warning: Invalid key format for command '" << cmd_str << "' in config." << std::endl;
                isValid = false;
                break; // Некорректный формат ключа
            }
            char key = std::toupper(key_val.get<std::string>()[0]);

            CommandType cmdType;
            if (cmd_str == "move_up") cmdType = CommandType::MOVE_UP;
            else if (cmd_str == "move_down") cmdType = CommandType::MOVE_DOWN;
            else if (cmd_str == "move_left") cmdType = CommandType::MOVE_LEFT;
            else if (cmd_str == "move_right") cmdType = CommandType::MOVE_RIGHT;
            else if (cmd_str == "switch_combat") cmdType = CommandType::SWITCH_COMBAT;
            else if (cmd_str == "ranged_attack") cmdType = CommandType::RANGED_ATTACK;
            else if (cmd_str == "cast_spell") cmdType = CommandType::CAST_SPELL;
            else if (cmd_str == "buy_spell") cmdType = CommandType::BUY_SPELL;
            else if (cmd_str == "save_game") cmdType = CommandType::SAVE_GAME;
            else if (cmd_str == "quit_game") cmdType = CommandType::QUIT_GAME;
            else {
                std::cout << "Warning: Unknown command name '" << cmd_str << "' in config." << std::endl;
                isValid = false;
                break; // Неизвестная команда
            }

            // Проверка на дубликаты
            if (newMap.find(key) != newMap.end()) {
                std::cout << "Warning: Duplicate key '" << key << "' found in config." << std::endl;
                isValid = false;
                break;
            }
            newMap[key] = cmdType;
        }

        // Проверка на дубликаты
        if (isValid) {
            std::set<CommandType> seenCmds;
            for (const auto& pair : newMap) {
                CommandType cmd = pair.second;
                if (seenCmds.count(cmd)) {
                     std::cout << "Warning: Duplicate command '" << static_cast<int>(cmd) << "' found in config." << std::endl;
                     isValid = false;
                     break;
                }
                seenCmds.insert(cmd);
            }
        }

        if (isValid) {
            keyMap = std::move(newMap);
            std::cout << "Controls loaded successfully from " << filepath << std::endl;
        } else {
            std::cout << "Config file " << filepath << " is invalid. Using defaults." << std::endl;
        }

    } catch (const nlohmann::json::parse_error& e) {
        std::cout << "Warning: JSON Parse error in control config file " << filepath << ": " << e.what() << std::endl;
    } catch (const nlohmann::json::type_error& e) {
        std::cout << "Warning: JSON Type error in control config file " << filepath << ": " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Warning: Error loading control config file " << filepath << ": " << e.what() << std::endl;
    }
}