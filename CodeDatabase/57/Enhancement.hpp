#pragma once
#include "AbstractSpell.hpp"
#include "InterfaceEnhancementSpell.hpp"
#include <iostream>

class EnhancementSpell : public Spell, public virtual InterfaceEnhancementSpell {
private:
    int bonusLevel = 1;

public:
    EnhancementSpell(int bonus = 1)
        : Spell("./sprites/lvelup.png"), bonusLevel(bonus) {}

    bool cast(Entity& caster, [[maybe_unused]] Field& field, InterfaceGameLogger& logger, [[maybe_unused]] int x, [[maybe_unused]] int y, [[maybe_unused]] Entity_Manager* manager = nullptr) override {
        caster.set_level_spell(bonusLevel + caster.get_level_spell());
        logger.log("Enhancement applied! Hero level is now " + std::to_string(caster.get_level_spell()) + "\n");
        return true;
    }


    std::string getName() const override { return "EnhancementSpell"; }
    sf::Sprite get_sprite() const override { return sprite; }
    bool requiresTarget() const override { return false; }
    void set_bonusLevel(int new_bonusLevel) { 
        this->bonusLevel = new_bonusLevel; 
    }
    
    int get_bonusLevel() const { 
        return bonusLevel; 
    }
    
    void upgrade(int new_level_spells = 1) {
        bonusLevel += new_level_spells;
    }

    int getbonusLevel() { return bonusLevel; }
};
