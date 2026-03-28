#pragma once
#include "AbstractSpell.hpp"
#include "InterfaceTrapSpell.hpp"
#include <iostream>

class TrapSpell : public Spell, public virtual InterfaceTrapSpell {
private:
    int trapDamage;

public:
    TrapSpell(int dmg = 20)
        : Spell("./sprites/trapspel.png"), trapDamage(dmg) {}

    bool cast( Entity& caster, Field& field, InterfaceGameLogger& logger, int targetX, int targetY,  [[maybe_unused]] Entity_Manager* manager = nullptr) override {
        if (targetX < 0 || targetY < 0 || targetX >= int(field.get_grid_size().x) || targetY >= int(field.get_grid_size().y))
            return false;

        auto& cell = field.get_cells()[targetX][targetY];

        if (cell->get_status_cell() == ordinary && !cell->Cell_get_use()) {
            field.placeTrap(targetX, targetY, trapDamage * caster.get_level_spell());
            logger.log("Trap placed at (" + std::to_string(targetX) + ", " +
                       std::to_string(targetY) + ") dealing " +
                       std::to_string(trapDamage * caster.get_level_spell()) + " damage on trigger.\n");
            caster.set_level_spell(1);
            logger.log("Hero level is now " + std::to_string(caster.get_level_spell()) + "\n");

        } else if (cell->get_status_cell() == ordinary && cell->Cell_get_use()) {
            
            logger.log("Trap instantly damaged " +
                       cell->Cell_get_entity()->get_name() + " for " +
                       std::to_string(trapDamage * caster.get_level_spell()) + "!\n");
            cell->Cell_get_entity()->change_helpth(trapDamage * caster.get_level_spell(), logger, field.get_cells());
            caster.set_level_spell(1);
            logger.log("Hero level is now " + std::to_string(caster.get_level_spell()) + "\n");
        } else {
            logger.log("Trap cannot be placed at (" +
                       std::to_string(targetX) + ", " + std::to_string(targetY) + ")\n");
        }

        return true;
    }

    int getTrapDamage() const override { return trapDamage; }

    std::string getName() const override { return "TrapSpell"; }
    sf::Sprite get_sprite() const override { return sprite; }
};
