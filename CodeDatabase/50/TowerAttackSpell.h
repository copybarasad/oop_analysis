#pragma once
#include "IDirectDamageSpell.h"
#include <iostream>

class TowerAttackSpell : public IDirectDamageSpell {
private:
    double damage;
    int range;
    int x;
    int y;

public:
    TowerAttackSpell(double damage, int range, int x, int y);

    // Информация о заклинании
    std::string getName() const override { return "TowerAttack"; }
    int getRange() const override { return range; }
    int getManaCost() const override { return 0; } // башня не тратит ману
    bool requiresTarget() const override { return false; }

    int getTowerX(){return x;};
    int getTowerY(){return y;};

    // Урон
    double getDamage() const override { return damage; }
    void setDamage(double dmg) override {damage = dmg;}

    // Проверка и применение
    bool canCast(const Field& field, const Player& player) const override;
    void cast(Field& field, Player& player) override;

    ISpell* clone() const override {return new TowerAttackSpell(*this);};
};
