#ifndef ENEMYSPAWNER_H
#define ENEMYSPAWNER_H
#include <vector>

class Field;
class Enemy;

class EnemySpawner {
public:
    EnemySpawner(int x, int y, int spawnRate, int health = 100);
    void processTurn(Field& field, std::vector<Enemy>& enemies);
    bool isAlive() const;
    void takeDamage(int amount);
    int getHealth() const;

    int getX() const;
    int getY() const;

private:
    int x;
    int y;
    int spawnRate;
    int turnsUntilSpawn;
    int health;
};

#endif