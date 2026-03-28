#include "enemy.hpp"

Enemy::Enemy(int x, int y, int hp_, int dmg) : Entity(x, y, Faction::Enemy, hp_, dmg) {}
