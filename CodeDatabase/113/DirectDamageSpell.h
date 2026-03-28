#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "Spell.h"

class DirectDamageSpell : public Spell {
private:
    int m_damage;

public:
    DirectDamageSpell(const std::string& name, int manaCost, int range, int damage);

    bool cast(Player& caster, GameField& field) override;
    std::string getDescription() const override;
};

#endif // DIRECTDAMAGESPELL_H