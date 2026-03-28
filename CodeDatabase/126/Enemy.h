#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
#include "../field/Position.h"

class Enemy : public GameObject {
    Position position;
    int health;
    int damage;
    int level;
    
public:
    Enemy(int x, int y, int lvl = 1);
    
    bool isAlive() const override;
    void takeDamage(int mount_damage) override;
    Position getPosition() const override;
    
    void moving(const Position& new_pos);
    
    int getHealth() const;
    int getDamage() const;
    int getLevel() const;
    
    std::string serialize() const override;
    static std::shared_ptr<Enemy> deserialize(const std::string& data);
};

#endif