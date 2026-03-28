#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "../SpawnSpell/SpawnSpell.hpp"
#include "../../../../../Entities/Inanimate/Trap/Trap.hpp"

class TrapSpell : public SpawnSpell {
    public:
    TrapSpell (Field& field, std::vector<unsigned int> stats, size_t range, std::pair<size_t, size_t> caster={0, 0}, std::pair<size_t, size_t> target={0, 0});

    CastResult cast () override;

    void upgradeSpell (size_t level) override;
};

#endif