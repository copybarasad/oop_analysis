#ifndef TOWER_H
#define TOWER_H

#include "building.h"
#include "field.h"
#include "direct_damage_spell.h"

class Tower : public Building {
public:
    Tower(int id, int health, int attackDamage, int attackRange, int cooldown);
    ~Tower();
    EntityType getType() const override;
    void tick();
    void tryAttack(Field& field);
    void tryAttackIfTower(class Field& field) override;
    Tower* asTower() override;
    char getDisplaySymbol() const override;
private:
    int attackDamage_;
    int attackRange_;
    int cooldown_;
    int cooldownCounter_;
    DirectDamageSpell* towerSpell_;
};

#endif
