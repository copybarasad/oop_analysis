#pragma once

#include "interfaces/IAreaDamageSpell.h"
#include "Field.h"
#include "objects/Player.h"
#include "objects/GameObject.h"

class AreaDamageSpell : public IAreaDamageSpell {
    std::string name = "AreaDamage";
    int range;
    int area;
    int damage;
    SpellType type = SpellType::AREA_DAMAGE;
public:
    AreaDamageSpell(int range = 5, int damage = 100, int area = 2)
        : range(range), damage(damage), area(area) {}

    bool use(Field& field, Player* caster, int targetX, int targetY) override {
        if (!isInRange(caster->getX(), caster->getY(), targetX, targetY)) {
            return false;
        }
        for (auto& obj : field.getObjects()) {
            if (!obj->isEnemy()) continue;
            int x = obj->getX();
            int y = obj->getY();
            if (x >= targetX && x < targetX + area 
                    && y >= targetY && y < targetY + area) {
                Enemy* enemy = static_cast<Enemy*>(obj.get());
                enemy->takeDamage(damage);
            }
        }
        return true;
    }

    std::string getName() override { return name; }
    int getRange() override { return range; }
    int getDamage() override { return damage; }
    SpellType getType() override { return type; } 
    int getAreaSize() override { return area; }

    bool isInRange(int casterX, int casterY, int targetX, int targetY) override {
        int dx = targetX - casterX;
        int dy = targetY - casterY;
        return (dx * dx + dy * dy) <= (range * range);
    }
};
