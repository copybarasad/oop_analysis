#ifndef TOWER_SPELL_HPP_INCLUDED
#define TOWER_SPELL_HPP_INCLUDED

#include "spell.hpp"
class TowerSpell : public Spell {
public:
    TowerSpell(int dmg, int r) : dmg_(dmg), radius_(r) {}

    std::string name() const override { return "tower"; }
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
    int radius_;
};

#endif
