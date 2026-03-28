#ifndef GAME_ENEMY_BUILDING_H
#define GAME_ENEMY_BUILDING_H

#include "../utils/Point.h"
#include <iostream>

namespace Game {

class EnemyBuilding {
public:
    EnemyBuilding(Utils::Point location, int spawnIntervalTurns, int initialHealth = 150);
    EnemyBuilding(std::istream& is); // Конструктор для загрузки

    const Utils::Point& getPosition() const;
    bool shouldSpawnEnemyThisTurn(); 
    void resetSpawnTimer();
    
    bool isAlive() const;
    void takeDamage(int amount);
    int getHealth() const;

    void serialize(std::ostream& os) const;

private:
    void deserialize(std::istream& is);

    Utils::Point position;
    int spawnInterval; 
    int turnsUntilSpawn;
    int health;
};

} 

#endif