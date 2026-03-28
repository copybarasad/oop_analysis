#include "GameField.h"
#include "AllSpells.h"
#include <iostream>

bool EnhancementSpell::cast_by_player(GameField& field, Player& player, int target_x, int target_y) {
    if (player.get_mana() < get_mana_cost()) {
        std::cout << "Not enough mana! Need " << get_mana_cost() << " mana." << std::endl;
        return false;
    }
    
    add_stack();

    SpellHand& spell_hand = player.get_spell_hand();
    
    for (int i = 0; i < spell_hand.get_size(); i++) {
        ISpell* spell = spell_hand.get_spell(i);
        if (spell && spell->get_type() != SpellType::ENHANCEMENT) {
            apply_enhancement(spell);
        }
    }

    player.add_mana(-get_mana_cost());
    
    std::cout << "Enhancement spell prepared! Stacks: " << stacks_ << std::endl;
    std::cout << "Your next spell will be enhanced!" << std::endl;
    
    return true;
}

void EnhancementSpell::apply_enhancement(ISpell* spell) {
    if (stacks_ == 0) return;
    
    switch (spell->get_type()) {
        case SpellType::DIRECT_DAMAGE: {
            DirectDamageSpell* direct_spell = static_cast<DirectDamageSpell*>(spell);
            direct_spell->set_range(direct_spell->get_range() + 1);
            std::cout << "Direct Damage enhanced! " << direct_spell->get_range() << " range" << std::endl;
            break;
        }
        case SpellType::AREA_DAMAGE: {
            AreaDamageSpell* area_spell = static_cast<AreaDamageSpell*>(spell);
            area_spell->set_area_size(area_spell->get_area_size() + 1);
            std::cout << "Area Damage enhanced! area size: " << area_spell->get_area_size()<< std::endl;
            break;
        }
        case SpellType::TRAP: {
            TrapSpell* trap_spell = static_cast<TrapSpell*>(spell);
            trap_spell->set_damage(trap_spell->get_damage() + 15);
            std::cout << "Trap enhanced! " << trap_spell->get_damage() << " damage" << std::endl;
            break;
        }
        case SpellType::SUMMON: {
            SummonSpell* summon_spell = static_cast<SummonSpell*>(spell);
            summon_spell->set_ally_count(summon_spell->get_ally_count() + 1);
            std::cout << "Summon enhanced! Allies: " << summon_spell->get_ally_count() << std::endl;
            break;
        }
        case SpellType::ENHANCEMENT:
            std::cout << "Enhancement spell cannot be enhanced!" << std::endl;
            return;
    }
}

bool EnhancementSpell::upgrade(){
    level_++;
    stacks_ += 1;
    mana_cost_ = std::max(5, mana_cost_ - 5);
    return true;
}