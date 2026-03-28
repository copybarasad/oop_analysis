#include "spell_store.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include "summon_spell.h"
#include "enhancement_spell.h"
#include "entity_manager.h"
#include <iostream>

SpellStore::SpellStore(EntityManager& manager) : entity_manager(manager) {
    initialize_spells();
}

void SpellStore::initialize_spells() {
    available_spells.clear();
    
    available_spells.push_back(std::make_unique<DirectDamageSpell>("Angel's Spear", 2, 4, 2));
    available_spells.push_back(std::make_unique<AreaDamageSpell>("Dance of the Storm", 2, 3, 2, 5));
    available_spells.push_back(std::make_unique<TrapSpell>("Cephalot", 1, 3, 5));
    available_spells.push_back(std::make_unique<SummonSpell>("Wish on a Star", 10, 1, entity_manager));
    available_spells.push_back(std::make_unique<EnhancementSpell>("Elevation", 15));
}

void SpellStore::display_available_spells(const Player& player) const {
    std::cout << "=== SPELL STORE ===" << std::endl;
    std::cout << "Your points: " << player.get_score() << std::endl;
    std::cout << "Spell slots: " << player.get_spell_hand().get_spell_count() << "/" << player.get_spell_hand().get_max_size() << std::endl;
    std::cout << std::endl;
    std::cout << "Available spells:" << std::endl;
    
    for (size_t i = 0; i < available_spells.size(); ++i) {
        const Spell* spell = available_spells[i].get();
        std::cout << i + 1 << ". " << spell->get_name() << " - " << spell->get_description() << " [Range: " << spell->get_range() << ", Cost: " << spell->get_cost() << " points]" << std::endl;
    }
    std::cout << "0. Back to game" << std::endl;
}

bool SpellStore::purchase_spell(int index, Player& player) {
    if (index < 0 || static_cast<size_t>(index) >= available_spells.size()) {
        std::cout << "Invalid spell selection!" << std::endl;
        return false;
    }

    if (player.get_spell_hand().is_full()) {
        std::cout << "Cannot purchase spell: hand is full! (" << player.get_spell_hand().get_spell_count() << "/" << player.get_spell_hand().get_max_size() << ")" << std::endl;
        return false;
    }

    const Spell* selected_spell = available_spells[index].get();
    int spell_cost = selected_spell->get_cost();

    if (player.get_score() < spell_cost) {
        std::cout << "Not enough points! Need " << spell_cost << ", but only have " << player.get_score() << std::endl;
        return false;
    }

    player.spend_points(spell_cost);

    auto spell_copy = available_spells[index]->clone();
    if (!spell_copy) {
        std::cout << "Error: Failed to create spell copy!" << std::endl;
        return false;
    }

    if (!player.get_spell_hand().add_spell(std::move(spell_copy))) {
        std::cout << "Error: Failed to add spell to hand!" << std::endl;
        return false;
    }

    std::cout << "Successfully purchased " << selected_spell->get_name() << " for " << spell_cost << " points!" << std::endl;
    std::cout << "Remaining points: " << player.get_score() << std::endl; 
    return true;
}

int SpellStore::get_spell_count() const {
    return available_spells.size();
}