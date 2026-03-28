#include "entity.h"

Entity::Entity(int health, Weapon* weapon, int x, int y)
    : health_(health), max_health_(health), position_x_(x), position_y_(y), 
      weapon_(weapon) {}

void Entity::take_damage(int damage) {
    health_ -= damage;
    if (health_ < 0) health_ = 0;
}

bool Entity::is_alive() const {
    return health_ > 0;
}

int Entity::get_health() const {
    return health_;
}

int Entity::get_damage() const {
    return weapon_ ? weapon_->get_damage() : 0;
}

int Entity::get_max_health() const {
    return max_health_;
}

int Entity::get_x() const {
    return position_x_;
}

int Entity::get_y() const {
    return position_y_;
}

WeaponType Entity::get_attack_type() const {
    return weapon_ ? weapon_->get_type() : WeaponType::Melee;
}

const Weapon* Entity::get_weapon() const {
    return weapon_;
}

void Entity::set_health(int health) {
    health_ = health;
    if (health_ > max_health_) health_ = max_health_;
}

void Entity::set_weapon(Weapon* weapon) {
    weapon_ = weapon;
}

void Entity::move(int dx, int dy) {
    position_x_ += dx;
    position_y_ += dy;
}

bool Entity::is_at_position(int x, int y) const {
    return position_x_ == x && position_y_ == y;
}

void Entity::save(std::ostream& os) const {
    os << health_ << " " << max_health_ << " " << position_x_ << " " << position_y_;
}

void Entity::load(std::istream& is) {
    is >> health_ >> max_health_ >> position_x_ >> position_y_;
    if (max_health_ < 1) max_health_ = 1;
    if (health_ > max_health_) health_ = max_health_;
    if (health_ < 0) health_ = 0;
}