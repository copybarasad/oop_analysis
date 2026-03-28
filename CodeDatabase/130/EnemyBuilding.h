#ifndef ENEMYBUILDING_H
#define ENEMYBUILDING_H

class EnemyBuilding {
private:
    int spawnCooldown;
    int turnCounter;
    int health_;
    int maxHealth_;

public:
    explicit EnemyBuilding(int cooldown = 9, int health = 100);

    bool canSpawn() const;
    void nextTurn();
    void resetTurn();
    
    // Здоровье
    int getHealth() const { return health_; }
    int getMaxHealth() const { return maxHealth_; }
    bool isAlive() const { return health_ > 0; }
    void takeDamage(int amount);
};

#endif