#include "EnemyTowerManager.h"
#include "../../entites/Spells/ISpell.h"
#include <random>
#include <limits>
#include <cmath>

bool EnemyTowerManager::SpawnEnemiesTowers(Field& field, int count_enemy_towers, int level) {
    int w, h;
    field.GetSize(w, h);
    int count_spawn_cells = field.GetCountSpawnCells();

    if (count_enemy_towers >= (count_spawn_cells - 1) / 3)
        return false;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dist(0, w - 1);
    std::uniform_int_distribution<> y_dist(0, h - 1);

    for (int i = 0; i < count_enemy_towers; i++) {
        int x = x_dist(gen);
        int y = y_dist(gen);
        auto enemy_tower = std::make_shared<EnemyTower>(10 * (level/ 2), 5*(level/ 2));
        enemies_towers_.push_back(enemy_tower);
        while ((x == 0 && y == 0) || !field.SetEntity(enemy_tower, x, y)) {
            x = x_dist(gen);
            y = y_dist(gen);
        }
    }
    return true;
}


bool EnemyTowerManager::EnemiesTowersTurn(Field& field) {
	bool flag = true;

	for (auto building_weak : enemies_towers_) {
		if (auto building = building_weak.lock()) {
			std::shared_ptr<ISpell> spell = building->canAttack();
			if (!spell)
				continue;

			int building_x, building_y;
			if (!field.GetPosEntity(building, building_x, building_y)) {
				flag = false;
				continue;
			}

			std::shared_ptr<Entity> target = findTarget(building_x, building_y, field);
			if (!target)
				continue;

			SpellContext context;
			context.caster = building;
			context.target = target;
			context.field = &field;

			spell->use(context);
		}
	}
	return flag;
}

void EnemyTowerManager::DeleteDeadEnemiesTowers(Field& field) {
    for (auto it = enemies_towers_.begin(); it != enemies_towers_.end();) {
        if (auto building = it->lock()) {
            if (building->IsDead()) {
                field.RemoteEntity(building);
                it = enemies_towers_.erase(it);
            } else {
                ++it;
            }
        } else {
            it = enemies_towers_.erase(it);
        }
    }
}

const std::vector<std::weak_ptr<EnemyTower>>& EnemyTowerManager::GetEnemiesTowers() const {
    return enemies_towers_;
}

std::shared_ptr<Entity> EnemyTowerManager::findTarget(int x, int y, Field& field) {
    int width, height;
    field.GetSize(width, height);

    std::shared_ptr<Entity> nearest = nullptr;
    double min_dist = std::numeric_limits<double>::max();

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            auto entity = field.GetEntity(j, i);
            if (!entity) continue;

            if (!entity->IsFriendly()) continue;

            // Считаем евклидово расстояние
            double dist = std::hypot(x - j, y - i);

            if (dist < min_dist) {
                min_dist = dist;
                nearest = entity;
            }
        }
    }

    return nearest;
}

void EnemyTowerManager::AddEntity(std::weak_ptr<EnemyTower> tower) {
	enemies_towers_.push_back(tower);
}