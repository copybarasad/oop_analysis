#ifndef LEVEL_GENERATOR_H
#define LEVEL_GENERATOR_H

#include "entity_manager.h"
#include "game_map.h"
#include <vector>

class LevelGenerator {
public:
    static void generateLevel(GameMap& map, EntityManager& entities,
                             int width, int height, int level = 1, int enemyCount = 2, int buildingCount = 1, int EnemyTowerCount = 2);
    static const int WALLPROCENT = 4;
    static const int LOWPROCENT = 8;
    static bool flag;

private:
    static void generateRandom(GameMap& map, int count, MapCell::Type cellType);
    static Position findRandomEmptyPosition(const GameMap& map);
    static bool isPositionValidForSpawn(const GameMap& map, const Position& pos);
    static bool canPlaceWall(const GameMap& map, const Position& pos);
    static void generateWalls(GameMap& map, int wallCount);
    template<typename T>
    static void spawnEntity(GameMap& map, std::vector<T*>& entityList, int level = 1);
};

#endif