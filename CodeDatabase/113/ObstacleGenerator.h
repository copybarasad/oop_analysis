#ifndef OBSTACLEGENERATOR_H
#define OBSTACLEGENERATOR_H

#include "GameField.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <random>
#include <vector>

class ObstacleGenerator {
private:
    std::mt19937 m_randomGen;

public:
    ObstacleGenerator();

    void generateObstacles(GameField& field,
        const std::vector<Enemy>& enemies,
        const std::vector<EnemyTower>& towers,
        int level);

private:
    bool isPositionOccupied(int x, int y,
        const std::vector<Enemy>& enemies,
        const std::vector<EnemyTower>& towers) const;
    bool isNearPlayerStart(int x, int y) const;
    bool isValidObstaclePosition(int x, int y, const GameField& field,
        const std::vector<Enemy>& enemies,
        const std::vector<EnemyTower>& towers) const;
};

#endif // OBSTACLEGENERATOR_H