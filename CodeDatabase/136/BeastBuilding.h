#ifndef BEASTBUILDING_H
#define BEASTBUILDING_H

class BeastBuilding {
private:
    int spawnCounter;
    int spawnInterval;
    bool isActive;
    int health;
    int maxHealth;

public:
    BeastBuilding(int interval, int health = 100);
    
    bool shouldSpawnEnemy() const;
    void update();
    void setActive(bool active);
    void takeDamage(int damage);
    bool isDestroyed() const;
    
    bool getIsActive() const;
    int getHealth() const;
    int getMaxHealth() const;
};

#endif