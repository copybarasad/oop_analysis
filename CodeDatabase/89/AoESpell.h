#ifndef AOE_SPELL_H
#define AOE_SPELL_H

#include "Spell.h"

class AoESpell : public Spell {
public:
    AoESpell(int damage);

    bool cast(Player& player, Direction direction,
              std::vector<std::unique_ptr<Enemy>>& enemies,
              std::vector<std::unique_ptr<EnemyTower>>& towers,
              GameGrid& grid) override;

    std::string getName() const override { return "AreaOfEffect"; }
    int getCost() const override { return 20; }

private:
    int damage;
};

#endif