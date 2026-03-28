#include "EnemySpawner.h"
#include "Field.h"
#include "Enemy.h"

EnemySpawner::EnemySpawner(int startX, int startY, int rate, int hp)
    : x(startX), y(startY), spawnRate(rate), turnsUntilSpawn(rate), health(hp) {}

int EnemySpawner::getX() const {return x;}
int EnemySpawner::getY() const {return y;}
bool EnemySpawner::isAlive() const {return health > 0;}
int EnemySpawner::getHealth() const {return health;}

void EnemySpawner::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void EnemySpawner::processTurn(Field& field, std::vector<Enemy>& enemies) {
    if (!isAlive()) return;

    if (turnsUntilSpawn > 0) {
        turnsUntilSpawn--;
        return;
    } 
    turnsUntilSpawn = spawnRate;

    int directions[8][2] = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (auto& dir : directions) {
        int spawnX = x + dir[0];
        int spawnY = y + dir[1];

        if (field.isValidPosition(spawnX, spawnY) && field.getCellType(spawnX, spawnY) == CellType::EMPTY) {
            enemies.emplace_back(spawnX, spawnY, 50, 5);
            field.setCellType(spawnX, spawnY, CellType::ENEMY);
            return;
        }
    }
}