#include "hand.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include "summon_spell.h"
#include "enhancement_spell.h"
#include "entity_manager.h"
#include "event_manager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Hand::Hand(int max_size) : max_size(max_size) {
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }
}

bool Hand::add_spell(std::unique_ptr<Spell> spell) {
    if (spells.size() >= max_size) {
        std::cout << "Cannot add spell: hand is full!" << std::endl;
        return false;
    }
    
    if (!spell) {
        std::cout << "Cannot add null spell!" << std::endl;
        return false;
    }

    EventManager::trigger(EventType::SpellAcquired, spell->get_name());
    
    spells.push_back(std::move(spell));
    std::cout << "Spell " << spells.back()->get_name() << " added to hand." << std::endl;
    return true;
}

bool Hand::remove_spell(int index) {
    if (index < 0 || index >= spells.size()) {
        std::cout << "Invalid spell index: " << index << std::endl;
        return false;
    }
    
    std::string spell_name = spells[index]->get_name();
    spells.erase(spells.begin() + index);
    return true;
}

void Hand::clear() {
    std::cout << "Clearing all spells from hand." << std::endl;
    spells.clear();
}

Spell* Hand::get_spell(int index) const {
    if (index < 0 || index >= spells.size()) {
        return nullptr;
    }
    return spells[index].get();
}

int Hand::get_spell_count() const { 
    return spells.size(); 
}

int Hand::get_max_size() const { 
    return max_size; 
}

bool Hand::is_full() const { 
    return spells.size() >= max_size; 
}

bool Hand::is_empty() const { 
    return spells.empty(); 
}

void Hand::display_spells() const {
    if (spells.empty()) {
        std::cout << "Hand is empty." << std::endl;
        return;
    }
    
    std::cout << "=== SPELLS IN HAND (" << spells.size() << "/" << max_size << ") ===" << std::endl;
    for (size_t i = 0; i < spells.size(); ++i) {
        const Spell* spell = spells[i].get();
        std::cout << i + 1 << ". " << spell->get_name() << " - " << spell->get_description() << " [Range: " << spell->get_range() << ", Cost: " << spell->get_cost() << "]" << std::endl;
    }
    std::cout << "=================================" << std::endl;
}

void Hand::add_random_spell(EntityManager& entity_manager) {
    if (is_full()) {
        std::cout << "Cannot add random spell: hand is full!" << std::endl;
        return;
    }
    
    int spell_type = std::rand() % 5;
    std::unique_ptr<Spell> new_spell;
    
    switch (spell_type) {
        case 0:
            new_spell = std::make_unique<DirectDamageSpell>("Angel's Spear", 2, 4, 0);
            break;
        case 1:
            new_spell = std::make_unique<AreaDamageSpell>("Dance of the Storm", 2, 3, 2, 0);
            break;
        case 2:
            new_spell = std::make_unique<TrapSpell>("Cephalot", 1, 3, 0);
            break;
        case 3:
            new_spell = std::make_unique<SummonSpell>("Wish on a Star", 0, 1, entity_manager);
            break;
        case 4:
            new_spell = std::make_unique<EnhancementSpell>("Elevation", 0);
            break;
        default:
            std::cout << "Invalid spell type generated!" << std::endl;
            return;
    }
    
    add_spell(std::move(new_spell));
}


void Hand::apply_enhancement_to_all(int enhancement_level) {
    if (enhancement_level <= 0) {
        return;
    }
    
    std::cout << "Applying enhancement level " << enhancement_level << " to all spells..." << std::endl;
    for (auto& spell : spells) {
        spell->apply_enhancement(enhancement_level);
    }
}

std::vector<TrapSpell*> Hand::get_trap_spells() const {
    std::vector<TrapSpell*> trap_spells;
    for (const auto& spell : spells) {
        if (auto trap = dynamic_cast<TrapSpell*>(spell.get())) {
            trap_spells.push_back(trap);
        }
    }
    return trap_spells;
}

void Hand::increase_max_size(int additional_slots) {
    if (additional_slots <= 0) {
        throw std::invalid_argument("Additional slots must be positive");
    }
    max_size += additional_slots;
    std::cout << "Spell slots increased to " << max_size << std::endl;
}