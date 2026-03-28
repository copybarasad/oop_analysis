#ifndef SPAWN_SPELL_H
#define SPAWN_SPELL_H

#include "../../RangedSpell/RangedSpell.hpp"

class SpawnSpell : public RangedSpell {
    protected:
    std::vector<unsigned int> stats;

    SpawnSpell (Field& field, size_t range, std::vector<unsigned int> stats, std::pair<size_t, size_t> caster, std::pair<size_t, size_t> target);

    public:
    std::vector<unsigned int> getStats () const;
    bool canCast (size_t targetX, size_t targetY) const override;
};

#endif