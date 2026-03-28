#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

class EnemyBuilding {
private:
    static const int DEFAULT_SPAWN_COOLDOWN = 8;
    static const int BUILDING_HEALTH = 6;

    int positionX;
    int positionY;
    int health;
    int spawnCooldown;
    int currentCooldown;

public:
    EnemyBuilding(int startX, int startY);
    
    void setPosition(int x, int y);
    void setHealth(int h);
    void setCooldown(int c);
    int getPositionX() const;
    int getPositionY() const;
    int getHealth() const;
    int getCurrentCooldown() const;
    int getSpawnCooldown() const;

    void takeDamage(int damageAmount);
    void update();
    bool canSpawnEnemy() const;
    void resetCooldown();

    bool isAlive() const;
};

#endif