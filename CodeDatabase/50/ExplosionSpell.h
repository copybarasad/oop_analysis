#pragma once
#include "IAreaDamageSpell.h"

class ExplosionSpell : public IAreaDamageSpell{
private:
    double damage;
    int range;
    int areaSize;
    int manaCost;
public:
    ExplosionSpell(double damage, int range, int manaCost);

    //Методы интерфейса направленного заклинания
    
    double getAreaSize() const override {return areaSize;};

    //Методы интерфейса заклинания
    double getDamage() const override {return damage;};
    void setDamage(double dmg) override {damage = dmg;}
    std::string getName() const override {return "Explosion";};
    int getRange() const override {return range;};
    int getManaCost() const override {return manaCost;};

    //  проверка возможности применения и сам эффект
    bool canCast(const Field& field, const Player& player) const override;
    void cast(Field& field, Player& player) override;

    ISpell* clone() const override {return new ExplosionSpell(*this);};
};