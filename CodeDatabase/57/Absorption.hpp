#pragma once
#include "AbstractSpell.hpp"
#include "InterfaceAreaDamageSpell.hpp"
#include <iostream>

class AbsorptionSpell : public Spell, public virtual InterfaceAreaDamageSpell {
private:
    int damage;
    sf::Vector2i areaSize;

public:
    AbsorptionSpell(int dmg = 15, sf::Vector2i size = {1, 1})
        : Spell("./sprites/arrayspell.png"), damage(dmg), areaSize(size) {}

    bool cast(Entity& caster, Field& field, InterfaceGameLogger& logger, int targetX, int targetY,  [[maybe_unused]] Entity_Manager* manager = nullptr) override {
        int totalDamage = damage;

        for (int dx = 0; dx < areaSize.x + caster.get_level_spell(); ++dx) {
            for (int dy = 0; dy < areaSize.y + caster.get_level_spell(); ++dy) {
                int cellX = targetX + dx;
                int cellY = targetY + dy;

                if (cellX >= 0 && cellX < int(field.get_grid_size().x) &&
                    cellY >= 0 && cellY < int(field.get_grid_size().y)) {

                    auto cell = field.get_cells()[cellX][cellY];
                    if (cell->Cell_get_use() &&
                        cell->Cell_get_entity()->get_status() != caster.get_status()) {

                        logger.log("Absorption spell hit " + 
                                   cell->Cell_get_entity()->get_name() +
                                   " for " + std::to_string(totalDamage) + " damage!\n");

                        cell->Cell_get_entity()->change_helpth(totalDamage, logger, field.get_cells());
                    }
                }
            }
        }

        logger.log("Absorption spell affected area " +
                   std::to_string(areaSize.x) + "x" + std::to_string(areaSize.y) +
                   " for " + std::to_string(totalDamage) + " damage each!\n");
        caster.set_level_spell(1);
        logger.log("Hero level is now " + std::to_string(caster.getLevel()) + "\n");
        return true;
    }


    std::string getName() const override { return "AbsorptionSpell"; }
    sf::Sprite get_sprite() const override { return sprite; }
};
