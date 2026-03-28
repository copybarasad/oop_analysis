#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "Spell.h"

class TrapSpell : public Spell {
public:
    TrapSpell(int damage);

    bool cast(Player& player, Direction direction,
              std::vector<std::unique_ptr<Enemy>>& enemies,
              std::vector<std::unique_ptr<EnemyTower>>& towers,
              GameGrid& grid) override;

    std::string getName() const override { return "Trap"; }
    int getCost() const override { return 15; }

private:
    int damage;
};

#endif