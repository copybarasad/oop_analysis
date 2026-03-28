#pragma once
#include "command.h"
#include "concrete_command.h"
#include "command_mapper.h"
#include <vector>
#include <iostream>

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
            "SHOW_MENU",
            "RIGHT", 
            "DOWN",
            "ATTACK",
            "DOWN",
            "RIGHT",
            "DOWN", 
            "ATTACK",
            "UP",
            "LEFT",
            "SHOW_SPELLS",
            "ATTACK"
        };
        
        std::cout << "=== ДЕМО-РЕЖИМ ===" << std::endl;
        std::cout << "Загружено " << commands.size() << " демо-команд" << std::endl;
        std::cout << "==================" << std::endl;
    }
    
    DemoInput() : currentCommand(0), demoCompleted(false), commandMapper(nullptr) {
        commands = {
            "SHOW_MENU",
            "RIGHT", 
            "DOWN",
            "ATTACK",
            "DOWN",
            "RIGHT",
            "DOWN", 
            "ATTACK",
            "UP",
            "LEFT",
            "SHOW_SPELLS",
            "ATTACK"
        };
    }
    
    template<typename RendererType>
    Command* readCommand() {
        if (demoCompleted) {
            std::cout << "[ДЕМО] Завершение демо-режима" << std::endl;
            return new QuitCommand();
        }
        
        if (currentCommand >= commands.size()) {
            if (!demoCompleted) {
                std::cout << "[ДЕМО] Все команды выполнены! Демо завершено." << std::endl;
                demoCompleted = true;
                return new QuitCommand();
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
    Command* createCommand(const std::string& commandName) {
        if (commandName == "UP") return new MoveUpCommand();
        if (commandName == "DOWN") return new MoveDownCommand();
        if (commandName == "LEFT") return new MoveLeftCommand();
        if (commandName == "RIGHT") return new MoveRightCommand();
        if (commandName == "ATTACK") return new AttackCommand();
        if (commandName == "SHOW_SPELLS") return new ShowSpellsCommand();
        if (commandName == "BUY_SPELL") return new BuySpellCommand();
        if (commandName == "SAVE") return new SaveCommand();
        if (commandName == "LOAD") return new LoadCommand();
        if (commandName == "QUIT") return new QuitCommand();
        if (commandName == "SHOW_MENU") return new ShowMenuCommand();
        if (commandName == "USE_SPELL") return new UseSpellCommand();
        if (commandName == "CHANGE_TYPE") return new ChangeCombatTypeCommand();
        if (commandName == "SHOW_STATUS") return new ShowStatusCommand();
        
        std::cout << "[ДЕМО] Неизвестная команда: " << commandName << std::endl;
        return nullptr;
    }
};