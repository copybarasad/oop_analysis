#pragma once
#include "ITrapSpell.h"
#include <vector>
#include <utility>
#include <iostream>

class TrapSpell : public ITrapSpell {
private:
    double damage;
    int range;
    int manaCost;

public:
    TrapSpell(double damage, int range, int manaCost);

    // Методы интерфейса
    std::string getName() const override { return "TrapSpell"; }
    int getRange() const override { return range; }
    int getManaCost() const override { return manaCost; }
    void setDamage(double dmg) override {damage = dmg;}

    bool canCast(const Field& field, const Player& player) const override;
    void cast(Field& field, Player& player) override;
    bool isValidPlacement(Field& field, int x, int y) override;

    double getDamage() const { return damage; }

    ISpell* clone() const override {return new TrapSpell(*this);};

    

};
