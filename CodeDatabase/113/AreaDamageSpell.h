#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "Spell.h"

class AreaDamageSpell : public Spell {
private:
    int m_damage;
    int applyDamageInArea(class Player& caster, int areaX, int areaY);

public:
    AreaDamageSpell(const std::string& name, int manaCost, int range, int damage);

    bool cast(Player& caster, GameField& field) override;
    std::string getDescription() const override;
};

#endif // AREADAMAGESPELL_H