#include "PlayerHand.h"
#include "FireballSpell.h"
#include "ZapSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BuffSpell.h"
#include <iostream>
#include <algorithm>
#include <random>

PlayerHand::PlayerHand(size_t size) : maxSize(size) {}

bool PlayerHand::addSpell(std::unique_ptr<ISpell> spell) {
    if (spells.size() < maxSize) {
        spells.push_back(std::move(spell));
        return true;
    }
    return false;
}

ISpell* PlayerHand::getSpell(size_t index) {
    if (index < spells.size()) {
        return spells[index].get();
    }
    return nullptr;
}

void PlayerHand::removeSpell(size_t index) {
    if (index < spells.size()) {
        spells.erase(spells.begin() + index);
    }
}
void PlayerHand::displaySpells() const {
    std::cout << "--- Your Spells ---" << std::endl;
    if (spells.empty()) {
        std::cout << "No spells available." << std::endl;
    } else {
        for (size_t i = 0; i < spells.size(); ++i) {
            std::cout << i << ": " << spells[i]->getName() << " - " << spells[i]->getDescription() << std::endl;
        }
    }
    std::cout << "-------------------" << std::endl;
}

size_t PlayerHand::getSpellCount() const {
    return spells.size();
}

void PlayerHand::removeRandomSpells(int count) {
    if (spells.empty()) return;
    int toRemove = std::min((int)spells.size(), count);
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(spells.begin(), spells.end(), g);
    
    spells.erase(spells.begin(), spells.begin() + toRemove);
}

void PlayerHand::save(std::ofstream& out) const {
    out << spells.size() << "\n";
    for (const auto& spell : spells) {
        out << static_cast<int>(spell->getType()) << " ";
    }
    out << "\n";
}

void PlayerHand::load(std::ifstream& in) {
    spells.clear();
    size_t count;
    if (!(in >> count)) return; 
    
    for (size_t i = 0; i < count; ++i) {
        int typeVal;
        in >> typeVal;
        std::unique_ptr<ISpell> spell;
        
        switch(static_cast<SpellType>(typeVal)) {
            case SpellType::FIREBALL: spell = std::make_unique<FireballSpell>(); break;
            case SpellType::ZAP: spell = std::make_unique<ZapSpell>(); break;
            case SpellType::TRAP: spell = std::make_unique<TrapSpell>(); break;
            case SpellType::SUMMON: spell = std::make_unique<SummonSpell>(); break;
            case SpellType::BUFF: spell = std::make_unique<BuffSpell>(); break;
            default: break;
        }
        if (spell) addSpell(std::move(spell));
    }
}