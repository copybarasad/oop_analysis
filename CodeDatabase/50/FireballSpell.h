#pragma once
#include "IDirectDamageSpell.h"

#include <vector>

class FireballSpell : public IDirectDamageSpell {
private:
    double damage;
    int range;
    int manaCost;

public:
    FireballSpell(double damage, int range, int manaCost);

    // Методы интерфейса направленного заклинания
    double getDamage() const override { return damage; }
    void setDamage(double dmg) override {damage = dmg;}

    // Методы интерфейса заклинания
    std::string getName() const override { return "FireBall"; }
    int getRange() const override { return range; }
    int getManaCost() const override { return manaCost; }
    bool requiresTarget() const override { return true; }

    bool canCast(const Field& field, const Player& player) const override;
    void cast(Field& field, Player& player) override;

    ISpell* clone() const override {return new FireballSpell(*this);};

};
