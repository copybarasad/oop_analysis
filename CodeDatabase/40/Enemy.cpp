#include "Enemy.h"
#include <iostream>

Enemy::Enemy(int enemyID, const std::string& enemyName, int initialHP, 
				CombatStyle style, int startX, int startY)
	: Character(enemyID, enemyName, initialHP, style, startX, startY) {}

void Enemy::move(int deltaX, int deltaY){
	if (!getisAlive()) return;
	posX += deltaX;
	posY += deltaY;
}

void Enemy::attack(Character& target){
	if (!getisAlive() || !target.getisAlive()) return;
	int damage = getDamage();
	std::cout << name << " attacks " << target.getName()
              << " for " << damage << " damage!\n";
	target.takeDamage(damage);
}

bool Enemy::canMoveTo(int targetX, int targetY) const {
	return getisAlive();
}

void Enemy::save(std::ostream& out) const {
    Character::save(out);
}

void Enemy::load(std::istream& in) {
    Character::load(in);
}