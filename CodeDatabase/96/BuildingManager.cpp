#include "BuildingManager.h"
#include <memory>
#include <vector>

BuildingManager::BuildingManager() = default;
BuildingManager::~BuildingManager() = default;

BuildingManager::BuildingManager(dataBuildingManager data){
    for (auto b:data.buildings){
        AddBuildingAtPos(b);
    }
}

void BuildingManager::AddBuildingAtPos(dataBuilding data) {
    buildings.emplace(data.pos, EnemyBuilding(data));
}

void BuildingManager::AddBuildingAtPos(std::pair<int,int> pos, int spawnInterval) {
    buildings.emplace(pos, EnemyBuilding(spawnInterval));
}

void BuildingManager::RemoveBuildingAtPos(std::pair<int,int> pos) {
    buildings.erase(pos);
}

int BuildingManager::GetBuildingSpawnInterval(std::pair<int,int> pos) const {
    auto it = buildings.find(pos);
    if (it == buildings.end()) return 0;
    return it->second.GetSpawnInterval();
}

std::vector<std::pair<int,int>> BuildingManager::UpdateBuildings() {
    std::vector<std::pair<int,int>> readySpawners;
    for (auto& [pos, building] : buildings) {
        if (building.Update()) {
            readySpawners.push_back(pos);
        }
    }
    return readySpawners;
}

bool BuildingManager::IsBuildingAtPos(std::pair<int,int> pos) const {
    return buildings.find(pos) != buildings.end();
}

void BuildingManager::TakeBuildingDamageAtPos(std::pair<int,int> pos, int amount) {
    auto it = buildings.find(pos);
    if (it != buildings.end()) {
        it->second.TakeDamage(amount);
        if (!it->second.IsAlive()) {
            buildings.erase(it);
        }
    }
}


bool BuildingManager::IsBuildiAliveAtPos(std::pair<int,int> pos) const {
    auto it = buildings.find(pos);
    if (it != buildings.end()) {
        return it->second.IsAlive();
    }
    return false;
}

dataBuildingManager BuildingManager::Save(){
    dataBuildingManager data;
    data.numBuildings=buildings.size();
    for (auto& [pos, building] : buildings) {
        dataBuilding datae = building.Save();
        datae.pos=pos;
        data.buildings.push_back(datae);
    }
    return data;
}