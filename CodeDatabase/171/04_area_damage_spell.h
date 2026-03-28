#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "02_spell_card.h"
#include "01_position.h"
#include <vector>

class Enemy; 

class AreaDamageSpell : public SpellCard {
public:
    AreaDamageSpell(int damage, int range, int cost = 20);

    bool cast(const Position& casterPos, const Position& targetPos) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getRange() const override;
    int getCost() const override;
    bool canUse() const override;

    std::vector<Position> getArea(const Position& center) const;

    void setEnemiesInArea(const std::vector<Enemy*>& enemies);

private:
    int damage_;
    int range_;
    int cost_;
    std::vector<Enemy*> enemiesInArea_; 
};

#endif