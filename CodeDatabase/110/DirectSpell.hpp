#ifndef DIRECT_SPELL_H
#define DIRECT_SPELL_H

#include "../DamageSpell/DamageSpell.hpp"

class DirectSpell : public DamageSpell {
    public:
    DirectSpell (Field& field, unsigned int damage, size_t range, std::pair<size_t, size_t> caster={0, 0}, std::pair<size_t, size_t> target={0, 0});

    CastResult cast () override;

    void upgradeSpell (size_t level) override;

    private:
    bool canCast (size_t targetX, size_t targetY) const override;
};

#endif