#ifndef CONSOLE_INPUT_PARSER_H
#define CONSOLE_INPUT_PARSER_H

#include "command.h"
#include "control_mapping.h"
#include "control_loader.h"
#include <iostream>
#include <string>

class ConsoleInputParser {
private:
    ControlMapping mapping_;

public:
    explicit ConsoleInputParser(const std::string& configPath = "controls.cfg")
        : mapping_(ControlLoader::loadFromFile(configPath)) {}

    explicit ConsoleInputParser(const ControlMapping& mapping)
        : mapping_(mapping) {}

    Command parseInput() {
        std::cout << "\nEnter command: ";
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) 
            return std::monostate{};

        char ch = line[0];
        auto it = mapping_.keyToCommand.find(ch);
        if (it == mapping_.keyToCommand.end()) 
            return std::monostate{};

        switch (it->second) {
            case CommandId::MOVE_UP:       return MoveCommand{0, -1};
            case CommandId::MOVE_DOWN:     return MoveCommand{0, 1};
            case CommandId::MOVE_LEFT:     return MoveCommand{-1, 0};
            case CommandId::MOVE_RIGHT:    return MoveCommand{1, 0};
            case CommandId::ATTACK_UP:     return AttackCommand{0, -1};
            case CommandId::ATTACK_DOWN:   return AttackCommand{0, 1};
            case CommandId::ATTACK_LEFT:   return AttackCommand{-1, 0};
            case CommandId::ATTACK_RIGHT:  return AttackCommand{1, 0};
            case CommandId::SWITCH_MODE:   return SwitchModeCommand{};
            case CommandId::BUY_SPELL:     return BuySpellCommand{};
            case CommandId::EXIT_GAME:     return ExitCommand{};
            case CommandId::SAVE_GAME:     return handleSave();
            case CommandId::USE_SPELL_1:   return handleSpell(0);
            case CommandId::USE_SPELL_2:   return handleSpell(1);
            case CommandId::USE_SPELL_3:   return handleSpell(2);
        }
        return std::monostate{};
    }

    void renderMessage(const std::string& msg) const {
        std::cout << msg << std::endl;
    }

private:
    Command handleSave() {
        std::cout << "Enter save filename: ";
        std::string filename;
        std::getline(std::cin, filename);
        return SaveGameCommand{filename.empty() ? "save.txt" : filename};
    }

    Command handleSpell(int idx) {
        std::cout << "Enter target coordinates (x y): ";
        int x, y;
        if (std::cin >> x >> y) {
            std::cin.ignore(1000, '\n');
            return UseSpellCommand{idx, x, y};
        }
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        return std::monostate{};
    }
};

#endif