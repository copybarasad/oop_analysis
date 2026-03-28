#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "Spell.h"

class DirectDamageSpell : public Spell {
public:
    DirectDamageSpell(int damage = 20); 

    bool cast(Player& player, Direction direction,
              std::vector<std::unique_ptr<Enemy>>& enemies,
              std::vector<std::unique_ptr<EnemyTower>>& towers,
              GameGrid& grid) override;

    std::string getName() const override { return "DirectDamage"; }
    int getCost() const override { return 15; }

private:
    int damage;
};

#endif