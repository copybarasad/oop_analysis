#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "IGameObject.h"
#include <algorithm>

class GameObject : public IGameObject {
protected:
    int m_health;
    int m_maxHealth;
    int m_positionX;
    int m_positionY;

public:
    GameObject(int health, int maxHealth, int x, int y);

    virtual ~GameObject() = default;

    // IGameObject interface
    int getHealth() const override { return m_health; }
    int getMaxHealth() const override { return m_maxHealth; }
    void takeDamage(int amount) override {
        m_health = std::max(0, m_health - amount);
    }
    int getPositionX() const override { return m_positionX; }
    int getPositionY() const override { return m_positionY; }
    bool isAlive() const override { return m_health > 0; }

    // Дополнительные методы
    void setPosition(int x, int y) { m_positionX = x; m_positionY = y; }
};

#endif // GAMEOBJECT_H