#include "SpellHand.h"
#include "ISpell.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

SpellHand::~SpellHand() {
    for (auto spell : spells_) {
        delete spell;
    }
}

bool SpellHand::add_spell(ISpell* spell) {
    if (is_full()) {
        delete spell;
        return false;
    }
    spells_.push_back(spell);
    return true;
}

bool SpellHand::remove_spell(int index){
    if (index < 0 || index >= spells_.size()) {
        std::cout << "Invalid spell index!" << std::endl;
        return false;
    }
    delete spells_[index];
    spells_.erase(spells_.begin() + index);
    return true;
}

ISpell* SpellHand::get_spell(int index) const {
    if (index < 0 || index >= spells_.size()) return nullptr;
    return spells_[index];
}

bool SpellHand::cast_spell(GameField& field, Player& player, int target_x, int target_y, int index) {
    if (index < 0 || index >= spells_.size()) {
        std::cout << "Invalid spell index!" << std::endl;
        return false;
    }

    bool res = spells_[index]->cast_by_player(field, player, target_x, target_y);
    
    if (dynamic_cast<EnhancementSpell*>(spells_[index]) == nullptr) {
        for (auto spell : spells_) {
            if (spell && dynamic_cast<EnhancementSpell*>(spell)) {
                static_cast<EnhancementSpell*>(spell)->reset_stacks();
                break;
            }
        }
        reset_all_spells();
    }

    return res;
}

void SpellHand::reset_all_spells(){
    for (int i = 0; i < get_size(); i++) {
        if (spells_[i]) {
            spells_[i]->reset_to_base();
        }
    }
}

void SpellHand::fill_random_spells(int count) {
    int added = 0;
    while (added < count && !is_full()) {
        SpellType random_type = static_cast<SpellType>(std::rand() % static_cast<int>(SpellType::COUNT));
        
        ISpell* spell;
        switch (random_type) {
            case SpellType::DIRECT_DAMAGE: spell = new DirectDamageSpell(); break;
            case SpellType::AREA_DAMAGE: spell = new AreaDamageSpell(); break;
            case SpellType::TRAP: spell = new TrapSpell(); break;
            case SpellType::SUMMON: spell = new SummonSpell(); break;
            case SpellType::ENHANCEMENT: spell = new EnhancementSpell(); break;
        }
        
        if (spell) {
            add_spell(spell);
            added++;
        }
    }
}

void SpellHand::display_spells() const {
    std::cout << "=== SPELL HAND (" << spells_.size() << "/" << max_size_ << ") ===" << std::endl;
    for (size_t i = 0; i < spells_.size(); ++i) {
        std::cout << i + 1 << ". " << spells_[i]->get_name() 
                  << " (Mana: " << spells_[i]->get_mana_cost() 
                  << ", Range: " << spells_[i]->get_range() << ")" << std::endl;
        std::cout << "   " << spells_[i]->get_description() << std::endl;
    }
    if (spells_.empty()) {
        std::cout << "No spells in hand" << std::endl;
    }
}