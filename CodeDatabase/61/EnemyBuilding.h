#ifndef ENEMYBUILDING_H
#define ENEMYBUILDING_H

#include <vector>
#include <string>

class EnemyBuilding {
private:
    int positionX;
    int positionY;
    int spawnCooldown;
    int currentCooldown;
    int buildingId;
    int enemyCounter;
    
    void validateInvariant() const;

public:
    EnemyBuilding(int startX, int startY, int spawnTurns, int buildingId = 0);
    
    // Getters
    int getPositionX() const;
    int getPositionY() const;
    int getSpawnCooldown() const;
    int getCurrentCooldown() const;
    int getId() const;
    bool canSpawn() const;
    
    // Spawn management
    void updateCooldown();
    void resetCooldown();
    
    // Spawn logic
    virtual bool trySpawnEnemy(class GameField& field, std::vector<class Enemy>& enemies);
    virtual bool trySpawnEnemy(class GameField& field, std::vector<class Enemy>& enemies, int enemyHealth, int enemyDamage);
    
    // Utility methods
    bool findSpawnPosition(const class GameField& field, int& spawnX, int& spawnY) const;
    
    // Damage support
    void takeDamage(int damage);
    bool isDestroyed() const;
    
    // Serialization
    std::string serialize() const;
    bool deserialize(const std::string& data);
    
protected:
    int health;
    int maxHealth;
};

#endif // ENEMYBUILDING_H

