#ifndef CONCRETESPELL_H
#define CONCRETESPELL_H

#include "Spell.h"

class ConcreteSpell : public Spell {
protected:
    SpellType type;
    std::string name;
    int damage;
    int range;
    int area;
    int manaCost;

public:
    ConcreteSpell(SpellType type, const std::string& name, int damage, int range, int area, int manaCost);
    
    SpellType getType() const override;
    std::string getName() const override;
    int getDamage() const override;
    int getRange() const override;
    int getArea() const override;
    int getManaCost() const override;
    
    void enhanceDamage(int amount) override;
    void enhanceRange(int amount) override;
    void enhanceArea(int amount) override;
    void enhanceDuration(int amount) override;
    void enhanceSummonCount(int amount) override;
};

#endif