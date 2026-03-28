#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include <memory>
#include "GameObject.h"
#include "../field/Position.h"

class Player;

class EnemyTower : public GameObject {
    Position position;
    int health;
    int damage;
    int attackRange;
    bool canAttack;
    int level;
    
public:
    EnemyTower(int x, int y, int lvl = 1);
    
    bool isAlive() const override;
    void takeDamage(int amount) override;
    Position getPosition() const override;
    
    void attackIfPossible(const Position& playerPos, std::shared_ptr<Player> player);
    void resetAttack();
    
    int getHealth() const;
    int getLevel() const;
    
    std::string serialize() const override;
    static std::shared_ptr<EnemyTower> deserialize(const std::string& data);
};

#endif