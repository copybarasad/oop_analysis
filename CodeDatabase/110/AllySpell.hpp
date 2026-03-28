#ifndef ALLY_SPELL_H
#define ALLY_SPELL_H

#include "../SpawnSpell/SpawnSpell.hpp"
#include "../../../../../Entities/Ally/Ally.hpp"

class AllySpell : public SpawnSpell {
    private:
    unsigned int alliesNumber;

    public:
    AllySpell (Field& field, std::vector<unsigned int> stats, size_t range, unsigned int alliesNumber=1, std::pair<size_t, size_t> caster={0, 0}, std::pair<size_t, size_t> target={0, 0});

    CastResult cast () override;

    void upgradeSpell (size_t level) override;

    private:
    void findExtraPositions (std::vector<std::pair<size_t, size_t>>& spawnPositions);
};

#endif