#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "spell.h"

class AreaDamageSpell : public Spell {
public:
    AreaDamageSpell(int dmg = 4, int area = 2);
    bool Cast(Field& field, Player& player, const std::pair<int,int>& target,
              std::vector<Enemy>& enemies, std::vector<EnemyTower>& towers, std::vector<EnemyBase>& bases) override;
    std::string Name() const override;
    std::string Description() const override;
    
    void SetDamage(int damage);
    void SetArea(int area_size);
    int GetArea();
    int GetDamage();


private:
    int damage_;
    int area_size_;
};

#endif