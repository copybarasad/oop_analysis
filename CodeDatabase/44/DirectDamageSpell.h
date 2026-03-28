#pragma once
#include <vector>
#include "IDirectDamageSpell.h"

class Enemy;

class DirectDamageSpell : public IDirectDamageSpell {
public:
    DirectDamageSpell(int damage = 4, int radius = 2);
    std::string GetName() const override;
    bool Use(std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& enemy_buildings,
             int px, int py, Field& field) override;
    int GetTarget(int available_size) const override;

private:
    int damage_;
    int radius_;
};