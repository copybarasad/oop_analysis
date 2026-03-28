#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include "constants.h"

class Entity
{
public:
    explicit Entity(int health, int x_pos, int y_pos, int melee_damage);
    virtual ~Entity() = default;

    enum LifeStatus {
        ALIVE,
        DEAD
    };

    enum Effect
    {
        NO_EFFECT,
        DELAYED
    };

    int getXPos() const { return m_xPos; }
    int getYPos() const { return m_yPos; }
    int getHealth() const { return m_health; }
    Effect getEffect() const { return m_effect; }
    int getDamage() const { return m_meleeDamage; }
    void setEffect(Effect effect) { m_effect = effect; }
    void kill();
    void addHealth(int health);
    bool isAlive() const;
    bool isInRange(Entity& other, int range) const;

    virtual void takeDamage(int damage) = 0;
    virtual int attack(Entity& other) = 0;

protected:
    int m_health;
    int m_xPos, m_yPos;
    int m_meleeDamage;
    Effect m_effect;
    LifeStatus m_lifeStatus;
    static std::vector<std::pair<int, int>> directions;
};

#endif // ENTITY_H
