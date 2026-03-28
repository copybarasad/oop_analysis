#include "Enemy.h"
#include "Exceptions.h"

namespace Game {

Enemy::Enemy(Utils::Point initialPosition, int initialHealth, int initialDamage) :
    position(initialPosition),
    health(initialHealth),
    damage(initialDamage)
{}

Enemy::Enemy(std::istream& is) {
    deserialize(is);
}

void Enemy::move(const Utils::Point& newPosition) {
    position = newPosition;
}

void Enemy::takeDamage(int damageAmount) {
    health -= damageAmount;
    if (health < 0) {
        health = 0;
    }
}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const {
    return damage;
}

const Utils::Point& Enemy::getPosition() const {
    return position;
}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&position), sizeof(position));
    os.write(reinterpret_cast<const char*>(&health), sizeof(health));
    os.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
}

void Enemy::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&position), sizeof(position));
    is.read(reinterpret_cast<char*>(&health), sizeof(health));
    is.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    if (is.fail()) throw FileReadException("Failed to read Enemy data.");
}

}