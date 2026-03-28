#include "characters/enemy.h"
#include "characters/player.h"

Enemy::Enemy(int health, int damage, Position position): Character(health, damage, position){}

void Enemy::attack(Player& player)
{
    player.takeDamage(this->getDamage());
}


