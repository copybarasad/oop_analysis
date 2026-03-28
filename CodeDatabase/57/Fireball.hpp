#pragma once
#include "AbstractSpell.hpp"
class Entity;
#include <iostream>
#include "InterfaceDirectDamageSpell.hpp"


class FireballSpell : public Spell, public virtual InterfaceDirectDamageSpell {
private:
    int damage;
    int radius; 

public:
    FireballSpell(int dmg = 30, int radius = 1)
        : Spell("./sprites/fireball.png"), damage(dmg), radius(radius) {}

    bool cast(Entity& caster, Field& field, InterfaceGameLogger& logger, int x, int y, [[maybe_unused]] Entity_Manager* manager = nullptr) override {
        if (!field.get_cells()[x][y]->Cell_get_use()) {
            logger.log(getName() + " couldn't launch a fireball because either it\n wasn't an opponent, or it was too far away.\n");
            return false;
        }

        auto* target = field.get_cells()[x][y]->Cell_get_entity();
        if (target->get_status() != caster.get_status() &&
            std::abs(x - caster.get_location().x) <= (radius + caster.get_level_spell()) &&
            std::abs(y - caster.get_location().y) <= (radius + caster.get_level_spell())) {
            int totalDamage = damage;
            logger.log(getName() + " hits " + target->get_name() + " for " + std::to_string(totalDamage) + " damage!\n");
            target->change_helpth(totalDamage, logger, field.get_cells());
            caster.set_level_spell(1);
            logger.log(caster.get_name() + " level is now " + std::to_string(caster.get_level_spell()) + "\n");
            return true;
        }
        logger.log(getName() + " couldn't launch a fireball because either it wasn't an opponent, or it was too far away.\n");
        
        return false;
    }

    int getDamage() const override { return damage; }
    int getRadius() const override { return radius; }

    std::string getName() const override { return "FireballSpell"; }
    
};
