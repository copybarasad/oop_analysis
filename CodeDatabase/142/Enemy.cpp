#include "Enemy.h"
#include "Player.h"
#include "Field.h"
#include <iostream>

Enemy::Enemy(int startX, int startY, int h, int d)
    : Entity(startX, startY, h), damage(d) {}

Enemy::Enemy(const Enemy& other)
    : Entity(other), damage(other.damage) {}

Enemy& Enemy::operator=(const Enemy& other) {
    if (this == &other)
        return *this;
    
    Entity::operator=(other);
    damage = other.damage;
    
    return *this;
}

Enemy::Enemy(Enemy&& other) noexcept
    : Entity(std::move(other)), damage(other.damage) {}

Enemy& Enemy::operator=(Enemy&& other) noexcept {
    if (this == &other)
        return *this;
    
    Entity::operator=(std::move(other));
    damage = other.damage;
    
    return *this;
}

bool Enemy::canMove(int dx, int dy, const Field& field) const {
    int newX = x + dx;
    int newY = y + dy;
    
    if (!field.isValidPosition(newX, newY))
        return false;
    
    if (field.isWall(newX, newY))
        return false;
    
    return true;
}

bool Enemy::move(int dx, int dy, Field& field) {
    if (!canMove(dx, dy, field))
        return false;
    
    int newX = x + dx;
    int newY = y + dy;
    
    if (field.hasPlayerAt(newX, newY)) {
        return attack(newX, newY, field);
    }
    
    field.clearCell(x, y);
    setPosition(newX, newY);
    field.placeEntity(this);
    
    return true;
}

bool Enemy::attack(int targetX, int targetY, Field& field) {
    auto player = field.getPlayer();
    if (!player || player->getX() != targetX || player->getY() != targetY)
        return false;
    
    player->takeDamage(damage);
    std::cout << "Enemy attacked player! Damage: " << damage << std::endl;
    
    return false;
}

int Enemy::getDamage() const {
    return damage;
}

char Enemy::getDisplayChar() const {
    return 'E';
}
