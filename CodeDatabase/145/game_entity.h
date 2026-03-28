#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <string>

class GameEntity {
protected:
    int health;
    int maxHealth;
    int damage;
    int score;
    
public:
    GameEntity(int health, int damage);
    virtual ~GameEntity() = default;
    
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getScore() const;
    void setHealth(int h);
    void setMaxHealth(int h);
    
    void takeDamage(int amount);
    void increaseScore(int points);
    
    bool isAlive() const;
};

#endif