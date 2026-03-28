#ifndef ENEMYBUILDING_H
#define ENEMYBUILDING_H

class EnemyBuilding {
private:
    int turnsUntilSpawn;
    int spawnInterval;
    int health;
    int maxHealth;

public:
    EnemyBuilding(int spawnInterval, int health = 100);
    
    int getTurnsUntilSpawn() const;
    int getSpawnInterval() const;
    int getHealth() const;
    int getMaxHealth() const;
    
    void processTurn();
    void setTurnsUntilSpawn(int turns);
    void resetSpawnTimer();
    bool shouldSpawn() const;
    
    int takeDamage(int damage);
    bool isAlive() const;
};

#endif
