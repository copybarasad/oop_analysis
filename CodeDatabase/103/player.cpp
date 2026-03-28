#include "player.h"

Player::Player(int startHealth, int startDamage, int startArmor)
    : Entity(startHealth, startDamage),
      armor(startArmor), maxHealth(100), points(0), isMelee(true) {
    if (startArmor < 0) {
        throw std::invalid_argument("Armor cannot be negative");
    }
    if (startHealth > 1000) {
        throw std::invalid_argument("Player health too high");
    }
    if (startHealth < 1) {
        throw std::invalid_argument("Player health too low");
    }
    if (startDamage < 1) {
        throw std::invalid_argument("Player damage too low");
    }
}

int Player::getArmor() const{ 
    return armor; 
}

void Player::setMaxHealth(int new_health){
    maxHealth = new_health;
}

void Player::setDamage(int dmg){
    damage = dmg;
}

int Player::getPoints() const{ 
    return points;
}

int Player::getMaxHealth() const{
    return maxHealth;
}

bool Player::getFightType() const{ 
    return isMelee;
}

void Player::takeDamage(int damage){
    if (damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    int damageByArmor = std::min(armor, damage / 2);
    int actualDamage = std::max(1, damage - damageByArmor);
    armor = std::max(0, armor - damageByArmor);
    health -= actualDamage;
    health = std::max(0, health);
}

void Player::heal(int amount){
    if (amount < 0) {
        throw std::invalid_argument("Heal amount cannot be negative");
    }
    health = std::min(maxHealth, health + amount);
}

void Player::addPoints(int newPoints){
    points += newPoints;
    if (points < 0) points = 0;
}

void Player::switchFightType(){
    if (isMelee) damage /= 2;
    else damage *= 2;
    isMelee = !isMelee;
}
