#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "iaimovable.h"

class Enemy : public Entity, public IAIMovable
{
public:
    enum TrajectoryChange
    {
        NO_CHANGE,
        CHANGE_NEEDED
    };

    enum AttackResult
    {
        MISSED,
        HIT
    };

    explicit Enemy(int health, int xPos, int yPos, int meleeDamage);
    ~Enemy() override = default;
    void trigger();
    void move(const Cell& cell, int dx, int dy) override;
    int attack(Entity& other) override;
    void takeDamage(int damage) override;
    std::pair<int, int> calculateMovement(int chaserX, int chaserY, int targetX, int targetY) override;

protected:
    TrajectoryChange m_pathFlag;
    AttackResult m_attackResult;
};

#endif // ENEMY_H
