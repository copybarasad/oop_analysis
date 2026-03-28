#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

#include <functional>
#include <stdexcept>

class EnemyBuilding {
private:
    int positionX;
    int positionY;
    int spawnCooldown;
    int currentCooldown;

    void validateCooldown(int cooldown) const {
        if (cooldown <= 0) {
            throw std::invalid_argument("Spawn cooldown must be positive");
        }
    }

public:
    EnemyBuilding(int x, int y, int cooldown, int maxX, int maxY);

    void getPosition(int &x, int &y) const;

    bool canSpawnEnemy() const;

    void update();

    void resetCooldown();

    bool trySpawnEnemy(int &spawnX, int &spawnY,
                       std::function<bool(int, int)> isPositionFree) const;

    int getSpawnCooldown() const;

    int getCurrentCooldown() const;
};

#endif
