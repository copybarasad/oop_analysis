#include "Character.h"

Character::Character(const Position &startPosition, int initialHealth, int initialDamage)
    : position(startPosition), health(initialHealth), maxHealth(initialHealth), damage(initialDamage) {}

Position Character::getPosition() const { return position; }
void Character::setPosition(const Position &newPosition) { position = newPosition; }
int Character::getHealth() const { return health; }
void Character::takeDamage(int damageAmount) { health -= damageAmount; if (health<0) health=0; }
bool Character::isAlive() const { return health>0; }
int Character::getDamage() const { return damage; }