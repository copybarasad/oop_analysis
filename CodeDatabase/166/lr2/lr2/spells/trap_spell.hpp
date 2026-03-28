#ifndef TRAP_SPELL_HPP_INCLUDED
#define TRAP_SPELL_HPP_INCLUDED

#include "spell.hpp"
#include "traps.hpp"

class TrapSpell : public Spell {
public:
    explicit TrapSpell(int dmg) : dmg_(dmg) {}

    std::string name() const override { return "trap"; }
    TargetMode targeting() const override { return TargetMode::Adjacent; }
    int radius() const override { return 1; }

    bool cast(Hero& hero,
              Field& field,
              std::vector<Enemy>& enemies,
              std::vector<Spawner>& spawners,
              BoostState& boosts,
              const Position& target) override;

private:
    int dmg_;
};

#endif
