#ifndef ENEMY_H
#define ENEMY_H

#include "Position.h"
#include <vector>

class Enemy {
private:
    Position position;
    int health = 50;
    int damage = 10;

public:
    Enemy(int startX, int startY);
    
    Enemy(const Enemy& other) = default;           
    Enemy(Enemy&& other) = default;              
    Enemy& operator=(const Enemy& other) = default; 
    Enemy& operator=(Enemy&& other) = default;      
    ~Enemy() = default;                           
    
    Position getPosition() const { 
        return position; 
    }
    int getHealth() const { 
        return health; 
    }
    int getDamage() const { 
        return damage; 
    }
    void move(const Position& playerPosition, const std::vector<std::vector<bool>>& passableMap,
             const std::vector<Enemy>& otherEnemies);
    void takeDamage(int damageAmount);
    bool isAlive() const { 
        return health > 0; 
    }
    void setPosition(const Position& newPosition) { 
        position = newPosition; 
    }

    void setHealth(int newHealth) { health = newHealth; }
    void setDamage(int newDamage) { damage = newDamage; }
    
    bool tryMode(int newX, int newY, const std::vector<std::vector<bool>>& passableMap,
             const std::vector<Enemy>& otherEnemies);
};

#endif