#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "spell.h"

class SummonSpell : public Spell {
public:
    SummonSpell(int count = 1);
    bool Cast(Field& field, Player& player, const std::pair<int,int>& target,
              std::vector<Enemy>& enemies, std::vector<EnemyTower>& towers, std::vector<EnemyBase>& bases) override;
    std::string Name() const override;
    std::string Description() const override;

    void SetCount(int count);
    int GetCount() const;

private:
    int ally_count_;
};

#endif