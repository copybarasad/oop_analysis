#pragma once

// Базовый класс для всех игровых сущностей
class Entity {
protected:
    int health;
    int damage;

public:
    Entity(int startHealth, int startDamage);
    virtual ~Entity() = default;

    // Геттеры
    int getHealth() const;
    int getDamage() const;
    
    // Сеттеры
    void takeDamage(int amount);
    void setDamage(int newDamage);
    void setHealth(int newHealth);
    
    // Состояние
    bool isAlive() const;
};
