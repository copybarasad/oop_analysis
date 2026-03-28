#pragma once
#include "Spell.h"

class TrapSpell : public Spell {
protected:
    std::string name;
    int cost;
    int range;
    int damage;

public:
    TrapSpell(std::string name, int cost, int range, int damage);
    void cast(Player& caster, std::vector<Enemy>& enemies, Field& field, int targetX, int targetY) override;
    std::string getName() const override;
    int getCost() const override;
    int getRange() const override;
    
    void save(std::ostream& os) const override;
    void load(std::istream& is) override;
};