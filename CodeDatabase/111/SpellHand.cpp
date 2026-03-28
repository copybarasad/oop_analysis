#include "SpellHand.h"
#include "GameBoard.h"
#include "TrapSpell.h"
#include "AreaDamageSpell.h"
#include <iostream>
#include <random>

SpellHand::SpellHand(size_t maximumSize, int initialMana) 
    : maxSize(maximumSize), mana(initialMana) {
}

bool SpellHand::addSpell(std::unique_ptr<Spell> spell) {
    if (isFull()) {
        std::cout << "Spell hand is full! Cannot learn new spell.\n";
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

bool SpellHand::removeSpell(size_t index) {
    if (index >= spells.size()) {
        return false;
    }
    spells.erase(spells.begin() + index);
    return true;
}

bool SpellHand::castSpell(size_t index, const Location& casterPos, const Location& targetPos, GameBoard& board) {
    std::cout << "DEBUG: SpellHand::castSpell called, index=" << index 
              << ", spells count=" << spells.size() << "\n";
    
    if (index >= spells.size()) {
        std::cout << "DEBUG: Invalid spell index\n";
        std::cout << "Invalid spell selection!\n";
        return false;
    }
    
    Spell* spell = spells[index].get();
    std::cout << "DEBUG: Got spell: " << spell->getName() << "\n";
    
    // Проверяем возможность применения (без проверки маны)
    std::cout << "DEBUG: Checking if spell can be cast\n";
    if (!spell->canCast(casterPos, targetPos, board)) {
        std::cout << "DEBUG: Spell cannot be cast at this location\n";
        std::cout << "Cannot cast spell at that location!\n";
        return false;
    }
    
    std::cout << "DEBUG: Spell can be cast\n";
    
    // Сохраняем имя заклинания ДО его использования
    std::string spellName = spell->getName();
    
    // Применяем заклинание (без траты маны)
    std::cout << "DEBUG: Calling spell->cast()\n";
    spell->cast(casterPos, targetPos, board);
    
    // УДАЛЯЕМ ЗАКЛИНАНИЕ ПОСЛЕ ИСПОЛЬЗОВАНИЯ
    std::cout << "DEBUG: Removing spell from hand\n";
    spells.erase(spells.begin() + index);
    std::cout << spellName << " was consumed and removed from your spell hand!\n";
    
    std::cout << "DEBUG: SpellHand::castSpell completed successfully\n";
    return true;
}

bool SpellHand::spendMana(int amount) {
    if (mana < amount) {
        std::cout << "Not enough mana! Need " << amount << " but have " << mana << ".\n";
        return false;
    }
    mana -= amount;
    std::cout << "Spent " << amount << " mana. Remaining: " << mana << "\n";
    return true;
}

const Spell* SpellHand::getSpell(size_t index) const {
    if (index >= spells.size()) {
        return nullptr;
    }
    return spells[index].get();
}

void SpellHand::displaySpells() const {
    std::cout << "Spells in hand:\n";
    for (size_t i = 0; i < spells.size(); ++i) {
        const Spell* spell = spells[i].get();
        std::cout << "  m" << (i + 1) << ". " << spell->getName() 
                  << " - " << spell->getDescription()
                  << " [Range: " << spell->getRange() << "]\n";
    }
    if (spells.empty()) {
        std::cout << "  No spells available - visit the shop to buy more!\n";
    }
}