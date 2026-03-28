#include "EnemyBuildingManager.h"
#include "EntityManager.h"
#include "../World.h"

#include <random>

bool EnemyBuildingManager::SpawnEnemiesBuildings(Field& field, int count_enemy_build, int level) {
    int w, h;
    field.GetSize(w, h);
    int count_spawn_cells = field.GetCountSpawnCells();

    if (count_enemy_build >= (count_spawn_cells - 1) / 2)
        return false;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dist(0, w - 1);
    std::uniform_int_distribution<> y_dist(0, h - 1);

    for (int i = 0; i < count_enemy_build; i++) {
        int x = x_dist(gen);
        int y = y_dist(gen);
        auto enemy_building = std::make_shared<EnemyBuilding>(2*(2 - level/50));
        enemies_buildings_.push_back(enemy_building);
        while ((x == 0 && y == 0) || !field.SetEntity(enemy_building, x, y)) {
            x = x_dist(gen);
            y = y_dist(gen);
        }
    }
    return true;
}

bool EnemyBuildingManager::EnemiesBuildingTurn(Field& field, World& world) {
    bool flag = true;
    for (auto building_weak : enemies_buildings_) {
        if (auto building = building_weak.lock()) {
            if (!building->CanSpawn())
                continue;

            int building_x, building_y;
            if (!field.GetPosEntity(building, building_x, building_y)) {
                flag = false;
                continue;
            }

            auto enemy = std::make_shared<Enemy>();
            int dx[4] = {-1, 1, 0, 0};
            int dy[4] = {0, 0, -1, 1};

            auto& enemies_manager = world.Enemies();
            for (int i = 0; i < 4; ++i) {
                if (field.SetEntity(enemy, building_x + dx[i], building_y + dy[i])) {
                    enemies_manager.AddEntity(enemy);
                    break;
                }
            }
        }
    }
    return flag;
}

void EnemyBuildingManager::DeleteDeadEnemiesBuildings(Field& field) {
    for (auto it = enemies_buildings_.begin(); it != enemies_buildings_.end();) {
        if (auto building = it->lock()) {
            if (building->IsDead()) {
                field.RemoteEntity(building);
                it = enemies_buildings_.erase(it);
            } else {
                ++it;
            }
        } else {
            it = enemies_buildings_.erase(it);
        }
    }
}

const std::vector<std::weak_ptr<EnemyBuilding>>& EnemyBuildingManager::GetEnemiesBuildings() const {
    return enemies_buildings_;
}

void EnemyBuildingManager::AddEntity(std::weak_ptr<EnemyBuilding> building) {
	enemies_buildings_.push_back(building);
}