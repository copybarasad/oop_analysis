#include "Entity.hpp"
#include "Configs.hpp"
#include <algorithm>

// Конструктор сущности
Entity::Entity(int startHealth, int startDamage) 
    : health(std::clamp(startHealth, Config::Constraints::MIN_HEALTH, Config::Constraints::MAX_HEALTH)),
      damage(std::clamp(startDamage, Config::Constraints::MIN_DAMAGE, Config::Constraints::MAX_DAMAGE)) {}

// Получение текущего здоровья
int Entity::getHealth() const {
    return health;
}

// Получение текущего урона
int Entity::getDamage() const {
    return damage;
}

// Получение урона
void Entity::takeDamage(int amount) {
    health = std::clamp(health - amount, 0, Config::Constraints::MAX_HEALTH);
}

// Установка нового значения урона
void Entity::setDamage(int newDamage) {
    damage = std::clamp(newDamage, Config::Constraints::MIN_DAMAGE, Config::Constraints::MAX_DAMAGE);
}

void Entity::setHealth(int newHealth) {
    health = std::clamp(newHealth, 0, Config::Constraints::MAX_HEALTH);
}

// Проверка, жива ли сущность
bool Entity::isAlive() const {
    return health > 0;
}
