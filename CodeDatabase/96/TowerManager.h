#pragma once
#include <vector>
#include <utility>
#include "map"
#include "Tower.h"
#include "struct/dataTowerManager.h"



class TowerManager {
private:
    std::map<std::pair<int,int>, Tower> towers;
public:
    TowerManager();
    ~TowerManager();

    TowerManager(dataTowerManager data);

    void AddTowerAtPos(std::pair<int,int> pos, int damageInterval, int damage);
    void RemoveTowerAtPos(std::pair<int,int> pos);
    int GetTowerDamageInterval(std::pair<int,int> pos) const;
    bool IsTowerAtPos(std::pair<int,int> pos) const;
    void TakeTowerDamageAtPos(std::pair<int,int> pos, int amount);
    const std::map<std::pair<int,int>, Tower>& GetTowers() const { return towers; }
    std::vector<std::pair<int,int>> UpdateTowers(const std::vector<std::pair<int,int>>& allyPos);
    bool IsTowerAliveAtPos(std::pair<int,int> pos) const;

    dataTowerManager Save();
};