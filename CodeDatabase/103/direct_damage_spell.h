#pragma once
#include "ispell.h"
#include "general_manager.h"
#include "enemy_manager.h"
#include "tower_manager.h"
#include "player_manager.h"

class DirectDamageSpell : public ISpell {
private:
    int damage;
    int cost;
    int radius;

public:
    DirectDamageSpell(int dmg = 20, int cost = 10, int r=3);
    std::string getName() const override;
    int getCost() const override;
    int getRadius() const override;
    void upgrade() override;
    int getDamage() const override;
    void cast(GeneralManager& genMan, std::pair<int, int> target) override;
};