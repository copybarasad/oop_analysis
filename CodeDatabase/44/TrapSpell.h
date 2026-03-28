#pragma once
#include "ITrapSpell.h"
#include <iostream>

class TrapSpell : public ITrapSpell {
public:
    static constexpr int kTrapDamage = 3;

    TrapSpell() = default;
    std::string GetName() const override;
    bool Use(std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& enemy_buildings,
             int px, int py, Field& field) override;
    std::pair<int, int> GetCoordsForTrap(Field& field) const override;
};
