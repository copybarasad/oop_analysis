#include "Character.hpp"
#include <iostream>

Character::Character(int x, int y, int health, int maxHealth, int damage)
    : positionX_(x), positionY_(y), health_(health), maxHealth_(maxHealth), damage_(damage) 
{
    validateHealth();
    if (damage_ < 0) {
        throw std::invalid_argument("Damage must be non-negative");
    }
}

void Character::validateHealth() {
    if (health_ < 0) {
        health_ = 0;
    }
    if (maxHealth_ <= 0) {
        throw std::invalid_argument("Max health must be positive");
    }
    if (health_ > maxHealth_) {
        health_ = maxHealth_;
    }
}

Character::Character(const Character& other)
    : health_(other.health_), maxHealth_(other.maxHealth_), damage_(other.damage_),
      positionX_(other.positionX_), positionY_(other.positionY_) 
{
    validateHealth();
}

Character& Character::operator=(const Character& other) {
    if (this != &other) {
        health_ = other.health_;
        maxHealth_ = other.maxHealth_;
        damage_ = other.damage_;
        positionX_ = other.positionX_;
        positionY_ = other.positionY_;
        validateHealth();
    }
    return *this;
}

Character::Character(Character&& other) noexcept
    : health_(other.health_), maxHealth_(other.maxHealth_), damage_(other.damage_),
      positionX_(other.positionX_), positionY_(other.positionY_) 
{
    other.health_ = 0;
    other.positionX_ = -1;
    other.positionY_ = -1;
    validateHealth();
}

Character& Character::operator=(Character&& other) noexcept {
    if (this != &other) {
        health_ = other.health_;
        maxHealth_ = other.maxHealth_;
        damage_ = other.damage_;
        positionX_ = other.positionX_;
        positionY_ = other.positionY_;
        
        other.health_ = 0;
        other.positionX_ = -1;
        other.positionY_ = -1;

        validateHealth();
    }
    return *this;
}

int Character::getDamage() const { 
    return damage_; 
}

int Character::getHealth() const { 
    return health_; 
}
    
int Character::getMaxHealth() const { 
    return maxHealth_; 
}
    
int Character::getPositionX() const { 
    return positionX_; 
}
    
int Character::getPositionY() const { 
    return positionY_; 
}
    
bool Character::isAlive() const { 
    return health_ > 0; 
}

void Character::takeDamage(int damage) {
    if (damage > 0) {
        health_ -= damage;
        validateHealth();
    }
}

void Character::setPosition(int x, int y) {
    positionX_ = x;
    positionY_ = y;
}

void Character::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&positionX_), sizeof(positionX_));
    os.write(reinterpret_cast<const char*>(&positionY_), sizeof(positionY_));
    os.write(reinterpret_cast<const char*>(&health_), sizeof(health_));
    os.write(reinterpret_cast<const char*>(&maxHealth_), sizeof(maxHealth_));
    os.write(reinterpret_cast<const char*>(&damage_), sizeof(damage_));
}

void Character::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&positionX_), sizeof(positionX_));
    is.read(reinterpret_cast<char*>(&positionY_), sizeof(positionY_));
    is.read(reinterpret_cast<char*>(&health_), sizeof(health_));
    is.read(reinterpret_cast<char*>(&maxHealth_), sizeof(maxHealth_));
    is.read(reinterpret_cast<char*>(&damage_), sizeof(damage_));
    validateHealth();
}