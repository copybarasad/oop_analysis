#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "spell.h"

class DirectDamageSpell : public Spell {
public:
    DirectDamageSpell(int dmg = 5, int radius = 2);
    bool Cast(Field& field, Player& player, const std::pair<int,int>& target,
              std::vector<Enemy>& enemies, std::vector<EnemyTower>& towers, std::vector<EnemyBase>& bases) override;
    std::string Name() const override;
    std::string Description() const override;

    void SetDamage(int damage);
    void SetRadius(int radius);
    int GetRadius();
    int GetDamage();

private:
    int damage_;
    int radius_;
};

#endif