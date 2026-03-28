#include "Enemy.h"

Enemy::Enemy() : 
    Creature(static_cast<char>(ObjectType::Enemy), ENEMY_HP, ENEMY_MELEE_DAMAGE)
{}
