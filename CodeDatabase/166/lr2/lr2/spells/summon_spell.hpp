#ifndef SUMMON_SPELL_HPP_INCLUDED
#define SUMMON_SPELL_HPP_INCLUDED

#include "spell.hpp"
#include "ally.hpp"

class SummonSpell : public Spell {
public:
    SummonSpell(int count, int hp=6, int dmg=2)   // союзники теперь по умолчанию с 6 hp
        : baseCount_(count), allyHp_(hp), allyDmg_(dmg) {}

    std::string name() const override { return "summon"; }
    TargetMode targeting() const override { return TargetMode::Adjacent; }
    int radius() const override { return 1; }

    bool cast(Hero& hero,
              Field& field,
              std::vector<Enemy>& enemies,
              std::vector<Spawner>& spawners,
              BoostState& boosts,
              const Position& target) override;

private:
    int baseCount_;
    int allyHp_;
    int allyDmg_;
};

#endif
