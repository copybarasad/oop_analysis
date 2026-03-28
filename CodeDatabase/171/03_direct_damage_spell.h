#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "02_spell_card.h"
#include "01_position.h"

class Enemy; 

class DirectDamageSpell : public SpellCard {
public:
    DirectDamageSpell(int damage, int range, int cost = 10);

    bool cast(const Position& casterPos, const Position& targetPos) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getRange() const override;
    int getCost() const override;
    bool canUse() const override;

    void setTargetEnemy(Enemy* enemy);

private:
    int damage_;
    int range_;
    int cost_;
    Enemy* currentTarget_; 
};

#endif