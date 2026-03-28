#include "Character.h"
#include <iostream>

Character::Character(std::string characterName, int startHealth, int startDamage, int startX, int startY)
    : name(characterName), health(startHealth), max_health(startHealth), 
      damage(startDamage), position{startX, startY}, alive(true), freeze_time(0), inSaloon(false) {}

void Character::take_damage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        alive = false;
    }
}

bool Character::isAlive() const { return alive; }

void Character::heal(int amount) { 
    health += amount;
    if (health > max_health) health = max_health;
    std::cout << "Ковбою "<< name <<" Восстановлено " << amount << " здоровья. Текущее здоровье: " << health << std::endl;
}

std::string Character::getName() const { return name; }
int Character::getHealth() const { return health; }
int Character::getMaxHealth() const { return max_health; }
int Character::getDamage() const { return damage; }
int Character::getFreeze_time() const { return freeze_time; }
int Character::getX() const { return position.x; }
int Character::getY() const { return position.y; }
bool Character::getInsaloon() const{ return inSaloon; }

void Character::setPosition(int newX, int newY) { 
    position.x = newX; 
    position.y = newY; 
}

void Character::setHealth(int newHealth) { 
    health = newHealth; 
    if (health > max_health) health = max_health;
}

void Character::setDamage(int newDamage) { damage = newDamage; }
void Character::setFreeze_time(int newFreze_time) {freeze_time = newFreze_time;}
void Character::setinSaloon(bool new_inSaloon){inSaloon = new_inSaloon;}
void Character::setMaxHealth(int newMaxHealth) { max_health = newMaxHealth; }
void Character::setAlive(bool isAlive) { alive = isAlive; }
void Character::setName(const std::string& newName) { name = newName; }