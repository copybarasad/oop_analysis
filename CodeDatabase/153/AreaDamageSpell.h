#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "SpellBase.h"

class AreaDamageSpell : public SpellBase {
public:
    AreaDamageSpell(const std::string& name, int cost, int spellRange, int spellDamage);

    bool cast(const SpellTarget& target, GameField& field,
        std::vector<Enemy>& enemies, Player& player) override;

    std::string getDescription() const override;
    std::unique_ptr<Spell> clone() const override;
};

#endif