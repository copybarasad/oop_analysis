#include "enemystructure.hpp"

EnemyStructure::EnemyStructure(int x, int y, int health, int damage, int delay) 
    : Entity(health, damage),
      x_(x), y_(y), delay_(delay), counter_(0) {}
