#pragma once
#include "IAreaDamageSpell.h"
#include "Enemy.h"
#include <iostream>

class AreaDamageSpell : public IAreaDamageSpell {
public:
    AreaDamageSpell(int damage = 1);

    std::string GetName() const override;
    bool Use(std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& enemy_buildings,
             int px, int py, Field& field) override;
    std::pair<int, int> GetAreaTopLeft(int field_width, int field_height) const override; 

private:
    int damage_;
};
