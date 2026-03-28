#pragma once
#include "AbstractSpell.hpp"
#include "./Entities/Knight.hpp"
#include <iostream>
#include "./Managers/Entity_Manager.hpp"
#include "InterfaceSummoningSpell.hpp"


class Arrise : public Spell, public virtual InterfaceSummoningSpell {
private:
    int radius;

public:
    Arrise(int radius = 5) : Spell("./sprites/allay.png"), radius(radius) {}

    bool cast(Entity& caster, Field& field, InterfaceGameLogger& logger_consol,
          int targetX, int targetY, Entity_Manager* manager = nullptr) override {
        (void)targetX;
        (void)targetY;


            
        std::cout << caster.get_level_spell() << std::endl;
        auto& cells = field.get_cells();
        int gridW = field.get_grid_size().x;
        int gridH = field.get_grid_size().y;

        int startX = caster.get_location().x;
        int startY = caster.get_location().y;

        int count = 0;
        int maxRadius = std::max(gridW, gridH);
        int currentRadius = 1;

        while (count < caster.get_level_spell() && currentRadius <= maxRadius) {
            bool summonedThisRound = false;

            for (int dx = -currentRadius; dx <= currentRadius; ++dx) {
                for (int dy = -currentRadius; dy <= currentRadius; ++dy) {

                    int cellX = startX + dx;
                    int cellY = startY + dy;

                    if (cellX < 0 || cellY < 0 || cellX >= gridW || cellY >= gridH)
                        continue;

                    if (!cells[cellX][cellY]->Cell_get_use() &&
                        cells[cellX][cellY]->get_status_cell() == ordinary)
                    {
                        Knight* new_knight = new Knight(cellX, cellY,
                            static_cast<int>(field.get_offset().x),
                            static_cast<int>(field.get_offset().y));

                        cells[cellX][cellY]->Cell_send_use(true, new_knight);

                        if (manager)
                            manager->addAlly(new_knight);

                        logger_consol.log("Knight summoned at (" +
                            std::to_string(cellX) + ", " +
                            std::to_string(cellY) + ").\n");

                        ++count;
                        summonedThisRound = true;

                        if (count >= caster.get_level_spell())
                            break;
                    }
                }
                if (count >= caster.get_level_spell())
                    break;
            }

            if (!summonedThisRound)
                ++currentRadius;
            else if (count >= caster.get_level_spell())
                break;
        }

        if (count > 0) {
            logger_consol.log("Summoned " + std::to_string(count) + " knights!\n");
            caster.set_level_spell(1);
            logger_consol.log("Hero level is now " + std::to_string(caster.get_level_spell()) + "\n");
            return true;
        }

        logger_consol.log("No free space for knights.\n");
        return false;
    }



    std::string getName() const override { return "Arrise"; }

    sf::Sprite get_sprite() const override { return sprite; }
    bool requiresTarget() const override { return false; }
    
};