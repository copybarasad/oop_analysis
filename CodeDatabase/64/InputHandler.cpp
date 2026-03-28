#include "InputHandler.h"
#include "Game.h"
#include <iostream>
#include <limits>

Command InputHandler::getCommand(const Game& game) {
    char input;
    std::cout << "Enter command (W/A/S/D - move, F - ranged, M - mode, C - spell, V - save, Q - quit): ";
    std::cin >> input;
    input = std::toupper(input);

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return Command(CommandType::INVALID);
    }

    switch (input) {
        case 'Q': return Command(CommandType::QUIT);
        case 'M': return Command(CommandType::SWITCH_STYLE);
        case 'V': return Command(CommandType::SAVE_GAME);
        
        case 'W': return Command(CommandType::MOVE, 0, -1);
        case 'S': return Command(CommandType::MOVE, 0, 1);
        case 'A': return Command(CommandType::MOVE, -1, 0);
        case 'D': return Command(CommandType::MOVE, 1, 0);
        
        case 'F': {
            int x, y;
            std::cout << "Enter target coordinates (X Y): ";
            std::cin >> x >> y;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return Command(CommandType::INVALID);
            }
            return Command(CommandType::ATTACK_RANGED, x, y);
        }
        
        case 'C': {
            int index;
            std::cout << "Choose spell index: ";
            std::cin >> index;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return Command(CommandType::INVALID);
            }

            int spellIdx = index - 1;
            const auto& hand = game.getPlayer().getSpellHand();
            
            if (spellIdx >= 0 && spellIdx < hand.getSpellCount()) {
                const auto& spell = hand.getSpells()[spellIdx];
                if (spell->getType() == SpellType::AREA_EFFECT) {
                    return Command(CommandType::CAST_SPELL, 0, 0, spellIdx);
                }
            }
            
            int x = 0, y = 0;
            std::cout << "Enter target coordinates (X Y): ";
            std::cin >> x >> y;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return Command(CommandType::INVALID);
            }
            
            return Command(CommandType::CAST_SPELL, x, y, spellIdx); 
        }
    }

    return Command(CommandType::INVALID);
}