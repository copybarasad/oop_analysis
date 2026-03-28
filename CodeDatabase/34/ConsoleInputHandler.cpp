#include "ConsoleInputHandler.h"
#include <iostream>
#include <limits>
#include "../Game.h"
#include "../commands/MoveCommand.h"
#include "../commands/CastSpellCommand.h"
#include "../commands/SwitchCombatCommand.h"
#include "../commands/SaveQuitCommand.h"
#include "../commands/QuitCommand.h"
#include "../commands/NullCommand.h"

namespace Game {

// Конструктор просто сохраняет ссылки, он больше не загружает файл
ConsoleInputHandler::ConsoleInputHandler(const Game& game, const KeyConfig& config) 
    : gameInstance(game), keyConfig(config) {}

void ConsoleInputHandler::displayPlayerSpells() const {
    std::cout << "Your spells:" << std::endl;
    const auto& spells = gameInstance.getPlayer().getSpellHand().getAllSpells();
    if (spells.empty()) {
        std::cout << "  (None)" << std::endl;
        return;
    }
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << spells[i]->getName() << " - " << spells[i]->getDescription() << std::endl;
    }
}

std::unique_ptr<ICommand> ConsoleInputHandler::getCommand() {
    std::cout << "Your action: ";
    char key;
    std::cin >> key;
    
    // Очистка буфера после любого cin >>
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    key = std::tolower(key);

    if (!keyConfig.isKeyBound(key)) {
        std::cout << "Unknown key." << std::endl;
        return std::make_unique<NullCommand>();
    }

    CommandType cmdType = keyConfig.getCommandType(key);

    switch (cmdType) {
        case CommandType::MoveUp:    return std::make_unique<MoveCommand>(-1, 0);
        case CommandType::MoveDown:  return std::make_unique<MoveCommand>(1, 0);
        case CommandType::MoveLeft:  return std::make_unique<MoveCommand>(0, -1);
        case CommandType::MoveRight: return std::make_unique<MoveCommand>(0, 1);
        case CommandType::SwitchCombat: return std::make_unique<SwitchCombatCommand>();
        case CommandType::SaveQuit:  return std::make_unique<SaveQuitCommand>();
        case CommandType::Quit:      return std::make_unique<QuitCommand>();
        case CommandType::CastSpell: {
            if (gameInstance.getPlayer().getSpellHand().getSpellCount() == 0) {
                std::cout << "You have no spells!" << std::endl;
                return std::make_unique<NullCommand>();
            }
            
            displayPlayerSpells();

            std::cout << "Choose a spell to cast (enter number, or 0 to cancel): ";
            int spellIndexChoice;
            std::cin >> spellIndexChoice;
            if (std::cin.fail() || spellIndexChoice < 0 || spellIndexChoice > gameInstance.getPlayer().getSpellHand().getSpellCount()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid spell choice or canceled." << std::endl;
                return std::make_unique<NullCommand>();
            }
            if (spellIndexChoice == 0) {
                 std::cout << "Casting canceled." << std::endl;
                 return std::make_unique<NullCommand>();
            }
            
            int spellIndex = spellIndexChoice - 1;
            
            const ISpell* spellToCheck = gameInstance.getPlayer().getSpellHand().getSpell(spellIndex);
            Utils::Point target(0, 0);

            if (spellToCheck && spellToCheck->requiresTarget()) {
                std::cout << "Enter target coordinates (row col): ";
                int r, c;
                std::cin >> r >> c;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid coordinates." << std::endl;
                    return std::make_unique<NullCommand>();
                }
                target.row = r;
                target.column = c;
            }
            
            return std::make_unique<CastSpellCommand>(spellIndex, target);
        }
    }
    return std::make_unique<NullCommand>();
}

}