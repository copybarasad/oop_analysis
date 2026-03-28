#include <iostream>
#include <cmath>
#include "area.h"
#include "game_field.h"
#include "cell.h"
#include "entity.h"
#include "prints.h"

AreaDamage::AreaDamage(int dmg, int radius, std::string name, SpellType id)
    : dmg(dmg), radius(radius), name(name), id(id) {}

void AreaDamage::cast(const CastContext &context) const
{
    GameField* field = Entity::currentField;
    int hitAnyEnemy = 0;
    int castX = context.targetX;
    int castY = context.targetY;
    Entity* gamer = context.gamer;

    if (castX < 0 || castY < 0 || castX >= field->getSizeX() || castY >= field->getSizeY()) {
        Prints::print_out_of_range_string();
        return;
    }

    Prints::print_area_use_string();

    for (int dy = -radius; dy <= radius; ++dy)
    {
        for (int dx = -radius; dx <= radius; ++dx) {
            int x = castX + dx;
            int y = castY + dy;
            if (x < 0 || y < 0 || x >= field->getSizeX() || y >= field->getSizeY()) continue;

            Cell& c = field->getCell(y, x);
             if (!c.hasEntity()) continue;

            Entity* target = c.getEntity();

            if (target && target->isAlive()) {
                int distX = std::abs(target->getPosX() - castX);
                int distY = std::abs(target->getPosY() - castY);
                int dist = std::max(distX, distY);

                if (dist <= radius)
                {

                    if (target->getGroup() != gamer->getGroup())
                    {
                        target->takeDamage(dmg);

                        Prints::print_take_damage(dmg, target->getName());
                        
                        if (!target->isAlive())
                        {
                            if(target->getGroup() == EVIL) {
                                gamer->setXp((gamer->getXp()) + 5);
                            }
                            Prints::print_enemy_dead(target->getName());
                            c.setEntity(nullptr);
                        }
                        hitAnyEnemy++;
                    }
            }
            
        }
    }
    }
    if(hitAnyEnemy==0){ Prints::print_spell_miss_string(); }
}

void AreaDamage::upgrade()
{
    dmg += 20; 
}

std::string AreaDamage::getName() const { return name; }
