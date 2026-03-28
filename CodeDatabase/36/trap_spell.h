#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "spell.h"

class TrapSpell : public Spell {
public:
    TrapSpell(int damage = 3);
    bool Cast(Field& field, Player& player, const std::pair<int,int>& target,
              std::vector<Enemy>& enemies, std::vector<EnemyTower>& towers, std::vector<EnemyBase>& bases) override;
    std::string Name() const override;
    std::string Description() const override;

private:
    int damage_;
};

#endif