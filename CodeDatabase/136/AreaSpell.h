#ifndef AREA_SPELL_H
#define AREA_SPELL_H

#include "Spell.h"

class AreaSpell : public ISpell {
private:
    std::string name;
    std::string description;
    int damage;
    int manaCost;
    int range;

public:
    virtual ~AreaSpell() = default;
    AreaSpell(std::string name, int damage, int manaCost = 3, int range = 2);
    
    void cast(Position target, GameField& field) override;
    std::string getName() const override { return name; }
    std::string getDescription() const override { return description; }
    int getManaCost() const override { return manaCost; }
    int getRange() const override { return range; }
    SpellType getType() const override { return SpellType::AREA_DAMAGE; }
};

#endif