#pragma once

#include "Entity.h"
#include "GameObject.h"
#include <memory>
#include "Config.h"

class Enemy : public Entity {
    int aggroRange;
public:
    Enemy(int x, int y, char sprite = Config::Enemy::SPRITE, std::string color = Config::Enemy::COLOR,
          int health = Config::Enemy::HEALTH, int damage = Config::Enemy::DAMAGE, int speed = Config::Enemy::SPEED, 
          int aggroRange = Config::Enemy::AGGRO_RANGE, ObjectTag tags = ObjectTag::TAG_ENEMY)
        : Entity(x, y, sprite, color, health, damage, speed, tags),
          aggroRange(aggroRange) {}
    
    int getAggroRange() const { return aggroRange; }

    void attack(Entity* target) {
        if (target && target->isAlive()) {
            target->takeDamage(damage);
        }
    }

    std::unique_ptr<GameObject> clone() const override {
        auto e = std::make_unique<Enemy>(
            x, y, sprite, color, health, damage, speed, aggroRange,
            static_cast<ObjectTag>(tags));
        e->setCanMove(this->canMove);
        return e;
    }
};
