#pragma once
#include "ispell.h"
#include "general_manager.h"
#include "enemy_manager.h"
#include "tower_manager.h"
#include "player_manager.h"

class AreaDamageSpell : public ISpell {
private:
    int damage;
    int cost;
    int radius;
public:
    AreaDamageSpell(int dmg = 10, int cost = 15, int r=3);
    std::string getName() const override;
    int getCost() const override;
    int getRadius() const override;
    int getDamage() const override;
    void upgrade() override;
    void cast(GeneralManager& genMan, std::pair<int, int> target) override;
};