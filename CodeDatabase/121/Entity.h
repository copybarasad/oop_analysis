#ifndef ENTITY_H
#define ENTITY_H

#include "Position.h"
#include "IDamageable.h"  // Наследуемся от интерфейса урона
#include <string>

// Базовый класс для всех игровых объектов: игрок, враги, здания, башни
// Реализует интерфейс IDamageable, поэтому любой наследник может получать урон
class Entity : public IDamageable {
protected:
    int health;           // Текущее здоровье объекта
    Position position;    // Позиция на игровом поле (x, y)
    std::string name;     // Имя объекта для отображения
    int reward;           // Очки которые получает игрок за уничтожение

public:
    // Конструктор с параметрами - теперь включает награду за уничтожение
    Entity(int health, const Position& pos, const std::string& entityName, int reward = 0);
    virtual ~Entity() = default;
    
    // Реализация методов интерфейса IDamageable
    void takeDamage(int damageAmount) override;
    bool isAlive() const override { return health > 0; }
    Position getPosition() const override { return position; }
    std::string getName() const override { return name; }
    int getReward() const override { return reward; }
    
    // Базовые геттеры и сеттеры
    int getHealth() const { return health; }
    void setHealth(int newHealth) { health = newHealth; }
    void setPosition(const Position& newPosition) { position = newPosition; }
    
    // Виртуальный метод обновления - для объектов с поведением (башни, здания)
    virtual void update() {}
    
    // Методы движения и расчета расстояний
    void move(int deltaX, int deltaY);
    double distanceTo(const Position& other) const;
    double distanceTo(const Entity& other) const;
    bool isAtPosition(const Position& pos) const;
    std::string positionToString() const;
};

#endif