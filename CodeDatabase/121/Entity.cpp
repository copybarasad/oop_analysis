#include "Entity.h"

// Конструктор Entity - инициализирует все поля объекта
Entity::Entity(int health, const Position& pos, const std::string& entityName, int reward) 
    : health(health), position(pos), name(entityName), reward(reward) {}

// Метод получения урона - уменьшает здоровье, но не ниже 0
void Entity::takeDamage(int damageAmount) {
    health -= damageAmount;
    if (health < 0) health = 0;
}

// Перемещение объекта по полю на указанные дельты
void Entity::move(int deltaX, int deltaY) {
    position.first += deltaX;
    position.second += deltaY;
}

// Расчет расстояния от этого объекта до другой позиции
double Entity::distanceTo(const Position& other) const {
    return PositionUtils::calculateDistance(position, other);
}

// Расчет расстояния от этого объекта до другого объекта
double Entity::distanceTo(const Entity& other) const {
    return distanceTo(other.getPosition());
}

// Проверка находится ли объект в указанной позиции
bool Entity::isAtPosition(const Position& pos) const {
    return position == pos;
}

// Преобразование позиции объекта в строку для отображения
std::string Entity::positionToString() const {
    return PositionUtils::toString(position);
}