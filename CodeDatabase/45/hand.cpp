#include "hand.hpp"
#include "spell.hpp"
#include <iostream>

#include "direct_spell.hpp"
#include "area_spell.hpp"
#include "trap_spell.hpp"

#include <random>
#include <memory>

Hand::Hand(int hand_capacity) {
    if (hand_capacity < 0) {
        capacity = 0;
    } else if (hand_capacity > 10) {
        capacity = 10;
    } else {
        capacity = hand_capacity;
    }
}

void Hand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() < static_cast<size_t>(capacity)) {
        spells.push_back(std::move(spell));
    }
}

void Hand::addRandomSpell() {
    if (spells.size() >= static_cast<size_t>(capacity)) {
        return;
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2);
    
    SpellType randomType = static_cast<SpellType>(dis(gen));
    std::unique_ptr<Spell> spell;
    
    switch (randomType) {
        case SpellType::Direct:
            spell = std::make_unique<DirectSpell>();
            break;
        case SpellType::Area:
            spell = std::make_unique<AreaSpell>();
            break;
        case SpellType::Trap:
            spell = std::make_unique<TrapSpell>();
            break;
    }
    
    spells.push_back(std::move(spell));
}

bool Hand::castSpell(int spellNumber, Map* map) {
    if (spellNumber < 0 || spellNumber >= static_cast<int>(spells.size())) {
        return false;
    }
    
    if (spells[static_cast<size_t>(spellNumber)]->cast(map)) {
        spells.erase(spells.begin() + spellNumber);
        return true;
    }
    
    return false;
}

void Hand::print_hand() {
    for (size_t i = 0; i < spells.size(); i++) {
        std::cout << "Spell " << i << ": ";
        SpellType st = spells[i]->getSpellType();
        switch (st) {
            case SpellType::Direct:
                std::cout << "Direct. ";
                break;
            case SpellType::Area:
                std::cout << "Area. ";
                break;
            case SpellType::Trap:
                std::cout << "Trap. ";
                break;
        }
    }
    std::cout << "\n";
}

std::string Hand::handToString() {
    std::string result;

    result += std::to_string(capacity) + "\n";
    
    for (size_t i = 0; i < spells.size(); i++) {
        SpellType st = spells[i]->getSpellType();
        switch (st) {
            case SpellType::Direct:
                result += 'D';
                break;
            case SpellType::Area:
                result += 'A';
                break;
            case SpellType::Trap:
                result += 'T';
                break;
        }
    }

    result += "\n";
    
    return result;
}

void Hand::remove_half_spells() {
    if (spells.empty()) {
        return;
    }
    
    size_t spells_to_remove = spells.size() / 2;
    
    if (spells_to_remove == 0) {
        return;
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    for (size_t i = 0; i < spells_to_remove; i++) {
        if (spells.empty()) {
            break;
        }
        
        std::uniform_int_distribution<size_t> dis(0, spells.size() - 1);
        size_t random_index = dis(gen);
        
        spells.erase(spells.begin() + random_index);
    }
}