#ifndef DIRECT_DAMAGE_SPELL_HPP_INCLUDED
#define DIRECT_DAMAGE_SPELL_HPP_INCLUDED

#include "spell.hpp"

class DirectDamageSpell : public Spell {
public:
    DirectDamageSpell(int dmg, int rad) : dmg_(dmg), baseRad_(rad) {}

    std::string name() const override { return "direct"; }
    TargetMode targeting() const override { return TargetMode::Cell; }
    int radius() const override { return baseRad_; }

    bool cast(Hero& hero,
              Field& field,
              std::vector<Enemy>& enemies,
              std::vector<Spawner>& spawners,
              BoostState& boosts,
              const Position& target) override;

private:
    int dmg_;
    int baseRad_;
};

#endif
