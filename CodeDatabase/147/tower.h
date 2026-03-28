#ifndef TOWER_H
#define TOWER_H

#include "unit.h"
#include "position.h"

// Специальный тип башни, которая может принимать урон, не двигается, и имеет
// внутренний spawn-cooldown. Спавн выполняется внешней логикой (controller/main).
class SpawnerTower : public Unit {
public:
    explicit SpawnerTower(int spawnCooldown = 3, int health = 200);


    // управление кулдауном извне
    void tick();                // декремент кулдауна
    bool canSpawn() const;      // готова ли спавнить
    void resetCooldown();       // сброс после удачного спавна

    int get_currentCooldown() const { return currentCooldown; }
    int getSpawnCooldown() const { return spawnCooldown; }
private:
    int spawnCooldown;
    int currentCooldown;
};

#endif // TOWER_H