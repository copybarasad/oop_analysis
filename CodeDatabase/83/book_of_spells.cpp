#include "book_of_spells.h"
#include "concrete_spells.h"
#include <cstdlib>
#include <iostream>
#include <ctime>

BookOfSpells::BookOfSpells() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    spellPrototypes.push_back(std::make_unique<FireballSpell>());
    spellPrototypes.push_back(std::make_unique<IceBladeSpell>());
    spellPrototypes.push_back(std::make_unique<IllusionSpell>());
    spellPrototypes.push_back(std::make_unique<EnlighteningSpell>());
}

std::unique_ptr<Spell> BookOfSpells::getRandomSpell() const {
    if (spellPrototypes.empty()) return nullptr;
    int index = std::rand() % spellPrototypes.size();
    return spellPrototypes[index]->clone();
}

void BookOfSpells::displayAllSpells() const {
    std::cout << "===== BOOK OF SPELLS =====\n";
    for (const auto& spell : spellPrototypes) {
        spell->display();
        std::cout << std::endl;
    }
    std::cout << "==========================\n";
}

std::unique_ptr<Spell> BookOfSpells::createSpellByName(const std::string& name) const {
    for (const auto& spell : spellPrototypes) {
        if (spell->getName() == name) {
            return spell->clone();
        }
    }
    return nullptr;
}