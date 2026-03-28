#pragma once
#include "ispell.h"
#include "cell_type.h"
#include "general_manager.h"
#include "enemy_manager.h"
#include "tower_manager.h"
#include "player_manager.h"

class TrapSpell : public ISpell {
private:
    int trapDamage;
    int cost;
    int radius;

public:
    TrapSpell(int dmg = 25, int cost = 12, int r=3);
    std::string getName() const override;
    int getCost() const override;
    int getRadius() const override;
    int getDamage() const override;
    void upgrade() override;
    void cast(GeneralManager& genMan, std::pair<int, int> target) override;
};