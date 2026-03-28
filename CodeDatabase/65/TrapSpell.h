#pragma once

#include "interfaces/ITrapSpell.h"
#include "Field.h"
#include "objects/Player.h"
#include "objects/GameObject.h"
#include "objects/Trap.h"

class TrapSpell : public ITrapSpell {
    std::string name = "Trap";
    int range;
    int damage;
    int areaSize;
    SpellType type = SpellType::TRAP;
    
public:
    TrapSpell(int range = 3, int damage = 50, int areaSize = 1)
        : range(range), damage(damage), areaSize(areaSize) {}

    bool use(Field& field, Player* caster, int targetX, int targetY) override {
        if (!field.isInBounds(targetX, targetY)) {
            return false;
        }

        if (!isInRange(caster->getX(), caster->getY(), targetX, targetY)) {
            return false;
        }

        GameObject* existingObject = field.getObjectAt(targetX, targetY);
        if (existingObject) {
            return false;
        }

        field.addObject(std::make_unique<Trap>(targetX, targetY, damage));
        return true;
    }

    std::string getName() override { return name; }
    int getRange() override { return range; }
    int getDamage() override { return damage; }
    SpellType getType() override { return type; } 
    int getAreaSize() override { return areaSize; }

    bool isInRange(int casterX, int casterY, int targetX, int targetY) override {
        int dx = targetX - casterX;
        int dy = targetY - casterY;
        return (dx * dx + dy * dy) <= (range * range);
    }
};
