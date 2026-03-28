#pragma once
#include "command.h"
#include "move_commands.h"
#include "action_commands.h"
#include "spell_commands.h"
#include "system_commands.h"
#include "command_mapper.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

class DemoInput {
private:
    std::vector<std::string> commands;
    size_t currentCommand;
    bool demoCompleted;
    CommandMapper* commandMapper;
    
public:
    DemoInput(std::istream& stream, CommandMapper& mapper) 
        : currentCommand(0), demoCompleted(false), commandMapper(&mapper) {
        
        commands = {
            "SHOW_STATUS",
            "RIGHT", 
            "DOWN",
            "ATTACK",
            "DOWN",
            "RIGHT",
            "DOWN", 
            "ATTACK",
            "UP",
            "LEFT",
            "CAST_SPELL",
            "ATTACK",
            "SAVE",
            "QUIT"
        };
        
        std::cout << "=== ДЕМО-РЕЖИМ ===" << std::endl;
        std::cout << "Загружено " << commands.size() << " демо-команд" << std::endl;
        std::cout << "==================" << std::endl;
    }
    
    DemoInput() : currentCommand(0), demoCompleted(false), commandMapper(nullptr) {
        commands = {
            "SHOW_STATUS",
            "RIGHT", 
            "DOWN",
            "ATTACK",
            "DOWN",
            "RIGHT",
            "DOWN", 
            "ATTACK",
            "UP",
            "LEFT",
            "CAST_SPELL",
            "ATTACK",
            "SAVE",
            "QUIT"
        };
    }
    
    std::unique_ptr<Command> readCommand() {
        if (demoCompleted) {
            std::cout << "[ДЕМО] Завершение демо-режима" << std::endl;
            return std::make_unique<QuitCommand>();
        }
        
        if (currentCommand >= commands.size()) {
            if (!demoCompleted) {
                std::cout << "[ДЕМО] Все команды выполнены! Демо завершено." << std::endl;
                demoCompleted = true;
                return std::make_unique<QuitCommand>();
            }
            return nullptr;
        }

        std::string commandStr = commands[currentCommand++];
        std::cout << "[ДЕМО] " << currentCommand << "/" << commands.size() 
                  << ": " << commandStr << std::endl;
        
        if (commandMapper) {
            std::string mappedCommand = commandMapper->mapInputToCommand(commandStr);
            if (!mappedCommand.empty()) {
                commandStr = mappedCommand;
            }
        }
        
        return createCommand(commandStr);
    }
    
    bool hasMoreCommands() const { return currentCommand < commands.size(); }
    void reset() { currentCommand = 0; demoCompleted = false; }
    size_t getTotalCommands() const { return commands.size(); }
    size_t getCurrentCommand() const { return currentCommand; }

private:
    std::unique_ptr<Command> createCommand(const std::string& commandName) {
        if (commandName == "UP" || commandName == "w") return std::make_unique<MoveUpCommand>();
        if (commandName == "DOWN" || commandName == "s") return std::make_unique<MoveDownCommand>();
        if (commandName == "LEFT" || commandName == "a") return std::make_unique<MoveLeftCommand>();
        if (commandName == "RIGHT" || commandName == "d") return std::make_unique<MoveRightCommand>();
        if (commandName == "ATTACK" || commandName == "k") return std::make_unique<AttackCommand>();
        if (commandName == "SWITCH_COMBAT" || commandName == "t") return std::make_unique<SwitchCombatCommand>();
        if (commandName == "CAST_SPELL" || commandName == "c") return std::make_unique<CastSpellCommand>();
        if (commandName == "BUY_SPELL" || commandName == "b") return std::make_unique<BuySpellCommand>();
        if (commandName == "SAVE" || commandName == "save") return std::make_unique<SaveGameCommand>();
        if (commandName == "LOAD" || commandName == "load") return std::make_unique<LoadGameCommand>();
        if (commandName == "SHOW_STATUS" || commandName == "status") return std::make_unique<ShowStatusCommand>();
        if (commandName == "QUIT" || commandName == "q") return std::make_unique<QuitCommand>();
        
        std::cout << "[ДЕМО] Неизвестная команда: " << commandName << std::endl;
        return nullptr;
    }
};