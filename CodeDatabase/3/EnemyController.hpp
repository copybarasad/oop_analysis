#pragma once
#include <vector>
#include "GameObjects/Entities/Enemy.hpp"
#include "GameClass/MapController.hpp"
#include "types.hpp"
#include <random>
#include <cmath>


class EnemyController {
public:
    EnemyController(int level, MapController& map);
    std::vector<EnemyDTO> toDTO();
    void loadLevel(int level, MapController& map);

    void addEnemy(Enemy& enemy, MapController& map);
    void makeAllStep(MapController& map, Player& player, StatisticDTO& stats);
    Enemy* getEnemy(Coordinates position);

    void makeActive();
    bool isActive();
private:
    bool needMove;
    LevelManager LM;
    std::vector<Enemy> enemies;
    
    Coordinates findBestWay(Coordinates enemyPos, Coordinates playerPos, std::vector<Coordinates> ways);
    void moveEnemy(Enemy& enemy, Coordinates oldPos, Coordinates newPos, MapController& map);
    void removeDeadEnemies(MapController& map, StatisticDTO& stats);
};
