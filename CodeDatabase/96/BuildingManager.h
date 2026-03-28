#pragma once
#include <utility>
#include <map>
#include <memory>
#include <vector>
#include "EnemyBuilding.h"
#include "struct/dataBuildingManager.h"


class BuildingManager {
private:
    std::map<std::pair<int,int>, EnemyBuilding> buildings;
    
public:
    BuildingManager();
    ~BuildingManager();

    BuildingManager(dataBuildingManager data);

    void AddBuildingAtPos(std::pair<int,int> pos, int spawnInterval);
    void AddBuildingAtPos(dataBuilding data);
    void RemoveBuildingAtPos(std::pair<int,int> pos);
    int GetBuildingSpawnInterval(std::pair<int,int> pos) const;
    bool IsBuildingAtPos(std::pair<int,int> pos) const;
    void TakeBuildingDamageAtPos(std::pair<int,int> pos, int amount);
    const std::map<std::pair<int,int>, EnemyBuilding>& GetBuildings() const { return buildings; }
    std::vector<std::pair<int,int>> UpdateBuildings();
    bool IsBuildiAliveAtPos(std::pair<int,int> pos) const;

    dataBuildingManager Save();
    
};