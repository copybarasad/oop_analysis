#include "GameManager.h"
#include "Entity.h"  

void GameManager::MoveEnemies(Map& map) {
    auto objects = map.GetObjects();
    for (auto& [coord, entity] : objects) {
        if (entity != nullptr && entity->ind_type == 'e') {
            entity->Move(map);
        }
    }
}