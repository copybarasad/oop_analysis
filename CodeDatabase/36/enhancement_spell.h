#ifndef ENHANCEMENT_SPELL_H
#define ENHANCEMENT_SPELL_H

#include "spell.h"

class EnhancementSpell : public Spell {
public:
    EnhancementSpell();
    bool Cast(Field& field, Player& player, const std::pair<int,int>& target,
              std::vector<Enemy>& enemies, std::vector<EnemyTower>& towers, std::vector<EnemyBase>& bases) override;
    std::string Name() const override;
    std::string Description() const override;
    void Enhance() override;
    bool CanBeEnhanced() const override;

private:
    int stacks_;
};

#endif