#pragma once
#include <map>
#include <vector>
#include "tower.h"

class TowerManager {
private:
    std::map<std::pair<int, int>, Tower*> towers;

public:
    ~TowerManager();
    void addTower(Tower* tower);
    bool hasTowerAt(int x, int y) const;
    void damageTowerAt(int x, int y, int dmg);
    void updateTowersCooldown();
    std::vector<std::tuple<int, int, int, int, int>> getTowersData() const;
    const std::map<std::pair<int, int>, Tower*>& getAllTowers() const;
};