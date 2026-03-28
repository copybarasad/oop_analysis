#pragma once
#include "Configs.hpp"
#include "Entity.hpp"

// Класс врага, наследуется от Entity
class Enemy : public Entity {
public:
    Enemy(int health = Config::Settings::ENEMY_START_HEALTH, 
          int damage = Config::Settings::ENEMY_START_DAMAGE);
};
