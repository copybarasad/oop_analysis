#include "Entity.h"
#include "Field.h"
#include "Coord.h"

Entity::Entity(int hp, int damage, std::string name)
        : hp_(hp), damage_(damage), name_(std::move(name)) {}

Entity::~Entity() = default;

void Entity::onTurn(Field& /*field*/, const Coord& /*pos*/) {
}

void Entity::takeDamage(int amount) {
    hp_ -= amount;
    if (hp_ < 0) hp_ = 0;
}

bool Entity::isAlive() const noexcept { return hp_ > 0; }
int Entity::hp() const noexcept { return hp_; }
int Entity::damage() const noexcept { return damage_; }
const std::string& Entity::name() const noexcept { return name_; }



