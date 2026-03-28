#include "characters/character.h"

Character::Character(int health, int damage, Position position)
    : health_(health), damage_(damage), position_(position), isSlow(false) {}

Character::~Character() = default;

void Character::takeDamage(int damage)
{
    if (isAlive())
        health_ -= damage;
}

bool Character::isAlive() { return health_ > 0; }
bool Character::thisIsSlow() {return isSlow;}
int Character::getDamage() const { return damage_; }
int Character::getHealth() const { return health_; }

Position Character::getPosition() { return position_; }

void Character::move(Position newPosition)
{
    if (newPosition.x >= 0 && newPosition.y >= 0)
        position_ = newPosition;
}
void Character::setHealth(int health)
{
        health_ = health;
}

void Character::setDamage(int damage) { damage_ = damage; }

void Character::setIsSlow(bool slow){isSlow = slow;}
