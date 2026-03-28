#include "character.hpp"

Character::Character(int startHealth, int startDamage, int startX, int startY) 
    : health(startHealth), maxHealth(startHealth), damage(startDamage), 
      x(startX), y(startY) {}

void Character::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Character::takeDamage(int damageAmount) {
    health -= damageAmount;
    if (health < 0) health = 0;
}

void Character::heal(int healAmount) {
    health += healAmount;
    if (health > maxHealth) health = maxHealth;
}
int Character::attack() { 
    return damage; 
}

void Character::setAblaze(){
    isBurn=true;
    timeLeft = 3;
} 

void Character::updateBurning(){
    timeLeft -= 1;
    
    if(!timeLeft) isBurn = false;

}