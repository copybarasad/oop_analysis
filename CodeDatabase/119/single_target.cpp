#include <iostream>
#include <cmath>
#include "single_target.h"
#include "game_field.h"
#include "cell.h"
#include "entity.h"
#include "prints.h"

SingleTarget::SingleTarget(int dmg, int radius, std::string name, SpellType id)
    : dmg(dmg), radius(radius), name(name), id(id){}

void SingleTarget::cast(const CastContext &context) const
{   
    int targetX = context.targetX;
    int targetY = context.targetY;
    Entity* gamer = context.gamer;
    GameField* field = Entity::currentField;
    bool hitAnyEnemy = false;


    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            int x = targetX + dx;
            int y = targetY + dy;
            if (x < 0 || y < 0 || x >= field->getSizeX() || y >= field->getSizeY())
                continue;

            Cell& c = field->getCell(y, x);
            if (!c.hasEntity()) continue;

            Entity* target = c.getEntity();

            if (target && target->isAlive()) {

                if ((target->getGroup() != gamer->getGroup()) && !hitAnyEnemy)
                {
                    target->takeDamage(dmg);
                    Prints::print_take_damage(dmg, target->getName());
                    if (!target->isAlive())
                    {   
                        if(target->getGroup() == EVIL) {
                            gamer->setXp((gamer->getXp()) + 5);
                        }
                        std::cout << target->getName() << " погиб!\n";
                        c.setEntity(nullptr);
                    }
                    hitAnyEnemy = true;
                }
                
            }
        }
    }
    if (!hitAnyEnemy) { Prints::print_spell_miss_string(); }
}

void SingleTarget::upgrade()
{
    dmg += 20; 
}

std::string SingleTarget::getName() const { return name; }
