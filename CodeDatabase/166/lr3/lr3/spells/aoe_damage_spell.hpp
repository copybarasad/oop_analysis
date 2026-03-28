#ifndef AOE_DAMAGE_SPELL_HPP_INCLUDED
#define AOE_DAMAGE_SPELL_HPP_INCLUDED

#include "spell.hpp"

class AoEDamageSpell : public Spell {
public:
    AoEDamageSpell(int dmg, int rad) : dmg_(dmg), baseRad_(rad) {}

    std::string name() const override { return "aoe"; }
    TargetMode targeting() const override { return TargetMode::Area2x2; }
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
