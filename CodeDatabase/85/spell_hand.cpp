#include "spell_hand.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "game_controller.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

SpellHand::SpellHand(int max_spells, int enemies_per_spell) 
    : max_size(max_spells),
      enemies_defeated(0),
      enemies_for_new_spell(enemies_per_spell)
{
    std::srand(std::time(nullptr));
    
    int spell_type = rand() % 2;
    std::unique_ptr<Spell> new_spell;
    
    if (spell_type == 0) {
        new_spell = std::make_unique<DirectDamageSpell>(
            "Magic Missile",
            "Shoots a bolt of magical energy",
            3,
            15
        );
    } else {
        new_spell = std::make_unique<AreaDamageSpell>(
            "Fireball",
            "Creates a fiery explosion",
            4,
            12,
            2
        );
    }
    
    spells.push_back(std::move(new_spell));
    std::cout << "Starting with spell: " << spells.back()->get_name() << std::endl;
}

bool SpellHand::add_spell(std::unique_ptr<Spell> spell) {
    if (spells.size() >= max_size) {
        std::cout << "Spell hand is full! Cannot add new spell." << std::endl;
        return false;
    }
    
    spells.push_back(std::move(spell));
    std::cout << "New spell added to hand: " << spells.back()->get_name() << std::endl;
    return true;
}

bool SpellHand::remove_spell(int index) {
    if (index < 0 || index >= spells.size()) {
        return false;
    }
    
    std::cout << "Spell removed: " << spells[index]->get_name() << std::endl;
    spells.erase(spells.begin() + index);
    return true;
}

void SpellHand::cast_spell(int index, Game_controller& controller, const Position& target) {
    if (index < 0 || index >= spells.size()) {
        std::cout << "Invalid spell index!" << std::endl;
        return;
    }
    
    spells[index]->cast(controller, target);
}

void SpellHand::on_enemy_defeated() {
    if (enemies_defeated < enemies_for_new_spell) {
        enemies_defeated++;
    }
    std::cout << "Enemies defeated: " << enemies_defeated << "/" << enemies_for_new_spell 
              << " for new spell\n";
}

bool SpellHand::can_get_new_spell() const {
    bool can_get = enemies_defeated >= enemies_for_new_spell && !is_full();
    std::cout << "Can get new spell: " << (can_get ? "YES" : "NO") 
              << " (defeated: " << enemies_defeated << "/" << enemies_for_new_spell 
              << ", full: " << (is_full() ? "YES" : "NO") << ")\n";
    return can_get;
}

void SpellHand::get_random_spell() {
    if (is_full()) {
        std::cout << "Spell hand is full, cannot get new spell!\n";
        return;
    }
    
    if (!can_get_new_spell()) {
        std::cout << "Not enough enemies defeated for new spell!\n";
        return;
    }
    
    int spell_type = rand() % 2;
    std::unique_ptr<Spell> new_spell;
    
    if (spell_type == 0) {
        new_spell = std::make_unique<DirectDamageSpell>(
            "Magic Missile",
            "Shoots a bolt of magical energy",
            3,
            15
        );
    } else {
        new_spell = std::make_unique<AreaDamageSpell>(
            "Fireball",
            "Creates a fiery explosion",
            4,
            12,
            2
        );
    }
    
    if (add_spell(std::move(new_spell))) {
        enemies_defeated = 0;
        std::cout << "Spell progress reset. Defeated: " << enemies_defeated 
                  << "/" << enemies_for_new_spell << "\n";
    }
}

void SpellHand::clear_spells() {
    spells.clear();
    enemies_defeated = 0;
}

void SpellHand::display_spells() const {
    std::cout << "=== Spell Hand ===" << std::endl;
    
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << i + 1 << ". " << spells[i]->get_name() 
                  << " (" << spells[i]->get_type() << ")" << std::endl;
        
        std::cout << "   " << spells[i]->get_description() << std::endl;
        
        std::cout << "   Range: " << spells[i]->get_range() << std::endl;
    }
    
    std::cout << "Enemies defeated: " << enemies_defeated << "/" 
              << enemies_for_new_spell << " for new spell" << std::endl;
    std::cout << "==================" << std::endl;
}