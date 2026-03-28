#include "Entity.h"
#include <algorithm>

Entity::Entity(int start_x, int start_y, int health, int damage)
    : health_(health), damage_(damage) {
    position_.x = start_x;
    position_.y = start_y;
}

Point Entity::getPosition() const {
    return position_;
}

void Entity::setPosition(int new_x, int new_y) {
    position_.x = new_x;
    position_.y = new_y;
}

int Entity::getHealth() const {
    return health_;
}

bool Entity::takeDamage(int amount) {
    bool was_alive = isAlive();
    health_ = std::max(0, health_ - amount);
    return was_alive && !isAlive();
}

int Entity::getDamage() const {
    return damage_;
}

bool Entity::isAlive() const {
    return health_ > 0;
}

void Entity::save(std::ostream& os) const {
    os << position_.x << " " << position_.y << " " << health_ << " " << damage_ << " ";
}

void Entity::load(std::istream& is) {
    is >> position_.x >> position_.y >> health_ >> damage_;
}