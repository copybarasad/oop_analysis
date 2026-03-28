#pragma once
#include "command.h"
#include "command_mapper.h"
#include "move_commands.h"
#include "action_commands.h"
#include "spell_commands.h"
#include "system_commands.h"
#include <iostream>
#include <memory>
#include <string>

class InputReader {
private:
    std::istream& inputStream;
    CommandMapper* commandMapper;
    
public:
    InputReader(std::istream& stream = std::cin, CommandMapper* mapper = nullptr) 
        : inputStream(stream), commandMapper(mapper) {}
    
    std::unique_ptr<Command> readCommand() {
        std::string input;
        std::getline(inputStream, input);
        
        if (input.empty()) {
            return nullptr;
        }
        
        std::string commandName = input;
        if (commandMapper) {
            commandName = commandMapper->mapInputToCommand(input);
            if (commandName.empty()) {
                commandName = input;
            }
        }
        
        return createCommand(commandName);
    }
    
private:
    std::unique_ptr<Command> createCommand(const std::string& commandName) {
        // Добавили проверки на MOVE_...
        if (commandName == "w" || commandName == "UP" || commandName == "MOVE_UP") {
            return std::make_unique<MoveUpCommand>();
        } else if (commandName == "s" || commandName == "DOWN" || commandName == "MOVE_DOWN") {
            return std::make_unique<MoveDownCommand>();
        } else if (commandName == "a" || commandName == "LEFT" || commandName == "MOVE_LEFT") {
            return std::make_unique<MoveLeftCommand>();
        } else if (commandName == "d" || commandName == "RIGHT" || commandName == "MOVE_RIGHT") {
            return std::make_unique<MoveRightCommand>();
        } else if (commandName == "k" || commandName == "ATTACK" || commandName == "f") { // f для атаки по умолчанию
            return std::make_unique<AttackCommand>();
        } else if (commandName == "t" || commandName == "SWITCH_COMBAT" || commandName == "SWITCH") {
            return std::make_unique<SwitchCombatCommand>();
        } else if (commandName == "c" || commandName == "CAST_SPELL" || commandName == "CAST") {
            return std::make_unique<CastSpellCommand>();
        } else if (commandName == "b" || commandName == "BUY_SPELL" || commandName == "BUY") {
            return std::make_unique<BuySpellCommand>();
        } else if (commandName == "save" || commandName == "SAVE") {
            return std::make_unique<SaveGameCommand>();
        } else if (commandName == "load" || commandName == "LOAD") {
            return std::make_unique<LoadGameCommand>();
        } else if (commandName == "status" || commandName == "SHOW_STATUS") {
            return std::make_unique<ShowStatusCommand>();
        } else if (commandName == "q" || commandName == "QUIT" || commandName == "EXIT") {
            return std::make_unique<QuitCommand>();
        }
        
        return nullptr;
    }
};