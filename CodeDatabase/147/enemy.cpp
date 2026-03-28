#include "enemy.h"

Enemy::Enemy(int health, int damage) : Unit(health, damage) {
	_type = CellType::ENEMY;
}
