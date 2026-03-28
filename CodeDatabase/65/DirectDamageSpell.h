#pragma once

#include "interfaces/IDirectDamageSpell.h"
#include "Field.h"
#include "objects/Player.h"
#include "objects/Enemy.h"
#include "objects/GameObject.h"

class DirectDamageSpell : public IDirectDamageSpell {
    std::string name = "DirectDamage";
    int range;
    int damage;
    int areaSize;
    SpellType type;
public:
    DirectDamageSpell(int range = 5, int damage = 100)
        : range(range), damage(damage), areaSize(1), type(SpellType::DIRECT_DAMAGE) {}
    bool use(Field& field, Player* caster, int targetX, int targetY) override {
        if (!field.isInBounds(targetX, targetY)) {
            return false;
        }

        if (caster && !isInRange(caster->getX(), caster->getY(), targetX, targetY)) {
            return false;
        }

        if (!caster) {
            field.getPlayer()->takeDamage(damage);
            return true;
        }

        GameObject* target = field.getObjectAt(targetX, targetY);

        if (target && target->hasTag(ObjectTag::TAG_ENEMY)) {
            Enemy* enemy = static_cast<Enemy*>(target);
            enemy->takeDamage(damage);
            return true;
        } else {
            return false;
        }

        return true;

    }

    virtual std::string getName() override { return name; }
    virtual int getRange() override { return range; }
    virtual int getDamage() override { return damage; }
    virtual SpellType getType() override { return type; }
    int getAreaSize() override { return areaSize; }

    bool isInRange(int casterX, int casterY, int targetX, int targetY) override {
        int dx = targetX - casterX;
        int dy = targetY - casterY;
        return (dx * dx + dy * dy) <= (range * range);
    }
};
