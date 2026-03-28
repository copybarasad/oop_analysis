#include <vector>
#include <utility>
#include "map"
#include "TowerManager.h"
#include <iostream>

TowerManager::TowerManager() = default;
TowerManager::~TowerManager() = default;

TowerManager::TowerManager(dataTowerManager data){
    for(auto t:data.towers){
        towers.emplace(t.pos, Tower(t));
    }
}

void TowerManager::AddTowerAtPos(std::pair<int,int> pos, int damageInterval, int damage) {
    towers.emplace(pos, Tower(damageInterval, damage));
}

void TowerManager::RemoveTowerAtPos(std::pair<int,int> pos) {
    towers.erase(pos);
}

int TowerManager::GetTowerDamageInterval(std::pair<int,int> pos) const {
    auto it = towers.find(pos);
    if(it != towers.end()) {
        return it->second.GetDamageInterval();
    }
    return -1; 
}

bool TowerManager::IsTowerAtPos(std::pair<int,int> pos) const {
    return towers.find(pos) != towers.end();
}

void TowerManager::TakeTowerDamageAtPos(std::pair<int,int> pos, int amount) {
    auto it = towers.find(pos);
    if(it != towers.end()) {
        it->second.TakeDamage(amount);
    }
}

std::vector<std::pair<int,int>> TowerManager::UpdateTowers(const std::vector<std::pair<int,int>>& allyPos) {
    std::vector<std::pair<int,int>> result;
    std::pair<int,int> damage;
    for(auto& [pos, tower] : towers) {
        if (result.size() == 0) {
            damage.first = 0;
            damage.second = tower.GetDamage(); 
            result.push_back(damage);
        }
        auto attackedPositions = tower.Update(allyPos, pos);
        for (auto pair : attackedPositions){
            //std::cout << "cyda yron " << pair.first << ' ' << pair.second <<'\n';
            result.push_back(pair);
        }

    }
    return result;
}

bool TowerManager::IsTowerAliveAtPos(std::pair<int,int> pos) const {
    auto it = towers.find(pos);
    if (it != towers.end()) {
        return it->second.IsAlive();
    }
    return false;
}

dataTowerManager TowerManager::Save(){
    dataTowerManager data;
    data.numTowers=towers.size();
    for (auto& [pos, tower] : towers) {
        dataTower datae = tower.Save();
        datae.pos=pos;
        data.towers.push_back(datae);
    }
    return data;
}