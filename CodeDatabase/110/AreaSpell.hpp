#ifndef AREA_SPELL_H
#define AREA_SPELL_H

#include "../DamageSpell/DamageSpell.hpp"

class AreaSpell : public DamageSpell {
    public:
    AreaSpell (Field& field, unsigned int damage, size_t range, std::pair<size_t, size_t> caster={0, 0}, std::pair<size_t, size_t> target={0, 0});

    DamageSpell::CastResult cast () override;

    void upgradeSpell (size_t level) override;

    private:
    bool canCast (size_t targetX, size_t targetY) const override;
    bool canSpread (size_t x, size_t y) const;
};

#endif