#include "UpgradeSpell.h"
#include "../game_main/GameState.h"
#include "../game_objects/Player.h"
#include "../interface/ConsoleRenderer.h"
#include <sstream>

UpgradeSpell::UpgradeSpell(int range) : range(range) { }

// Использовать заклинание
bool UpgradeSpell::use(Player& player, GameState& state, int targetX, int targetY) { 
    std::ostringstream ss;
    const PlayerHand& hand = state.getHand();

    int cnt = 0;
    for (std::size_t i = 0; i < hand.getCurrentSize(); ++i) {
        auto spell = hand.getSpell(i);
        if (spell->getSymbol() == 'U') 
            cnt++;
    }
    if (cnt == hand.getCurrentSize()) {
        ss << Colors::YELLOW << "No spells to upgrade in hand." << Colors::RESET;
        state.addMessage(ss.str());
        return false;
    }

    std::cout << Colors::PINK <<  "\n=== Upgrade Spell Modes ===\n" << Colors::RESET;
    std::cout << Colors::CYAN << "1. Improve ALL spells in hand (+1 level to each)\n";
    std::cout << "2. Improve specific spell (+" << UPGRADE_POWER << " levels)\n" << Colors::RESET;
    std::cout << "Choose mode (1 or 2): ";
    
    int mode;
    if (!(std::cin >> mode)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        ss << Colors::DARK_RED << "Invalid input." << Colors::RESET;
        state.addMessage(ss.str());
        return false;
    }

    if (mode == 1) {
        // Улучшаем ВСЕ заклинания в руке
        bool upgradedAny = false;
        
        for (size_t i = 0; i < hand.getCurrentSize(); ++i) {
            auto spell = hand.getSpell(i);
            if (spell && spell->getSymbol() != 'U') {
                int oldLevel = spell->getLevel();
                state.upgradeSpellInHand(i); // +1 уровень
                
                int newLevel = spell->getLevel();
                if (newLevel > oldLevel) {
                    upgradedAny = true;
                    ss << Colors::GREEN << spell->getName() << " upgraded to level " 
                       << newLevel << "!\n" << Colors::RESET;
                }
            }
        }
        
        if (!upgradedAny) {
            ss << Colors::YELLOW << "No spells to upgrade in hand." << Colors::RESET;
        } else {
            ss << Colors::GREEN << "All spells received +1 level!" << Colors::RESET;
        }
        
    } else if (mode == 2) {
        // Выбор конкретного заклинания для улучшения
        std::cout << Colors::PINK << "\n=== Choose spell to improve ===\n" << Colors::RESET;
        for (size_t i = 0; i < hand.getCurrentSize(); ++i) {
            auto spell = hand.getSpell(i);
            if (spell && spell->getSymbol() != 'U') {
                std::cout << Colors::CYAN << i + 1 << ". " << spell->getName() << "\n" << Colors::RESET;
            }
        }
        
        size_t targetSpellIndex;
        std::cout << "Enter number of spell to improve: ";
        
        if (!(std::cin >> targetSpellIndex)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            ss << Colors::DARK_RED << "Invalid input." << Colors::RESET;
            state.addMessage(ss.str());
            return false;
        }
        
        targetSpellIndex--;
        if (targetSpellIndex >= hand.getCurrentSize()) {
            ss << Colors::DARK_RED << "Wrong spell index." << Colors::RESET;
            state.addMessage(ss.str());
            return false;
        }
        
        auto targetSpell = hand.getSpell(targetSpellIndex);
        if (!targetSpell || targetSpell->getSymbol() == 'U') {
            ss << Colors::DARK_RED << "Cannot upgrade this spell." << Colors::RESET;
            state.addMessage(ss.str());
            return false;
        }

        // Улучшаем конкретное заклинание сильно
        int oldLevel = targetSpell->getLevel();
        for (int i = 0; i < UPGRADE_POWER; i++) {
            state.upgradeSpellInHand(targetSpellIndex);
        }
        int newLevel = targetSpell->getLevel();
        
        ss << Colors::GREEN << targetSpell->getName() << " upgraded from level " 
           << oldLevel << " to level " << newLevel << "!" << Colors::RESET;
    } else {
        ss << Colors::DARK_RED << "Invalid mode selected." << Colors::RESET;
        state.addMessage(ss.str());
        return false;
    }

    state.addMessage(ss.str());    
    return true; 
}

// Методы для улучшения
void UpgradeSpell::upgrade() { }

void UpgradeSpell::updateStats() { }

// Геттеры
std::string UpgradeSpell::getName() const { return "Upgrade Spell"; }

char UpgradeSpell::getSymbol() const { return 'U'; }

int UpgradeSpell::getRange() const { return range; }

int UpgradeSpell::getLevel() const { return 1; }