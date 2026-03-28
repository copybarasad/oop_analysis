#ifndef ARCHER_H
#define ARCHER_H

#include "entity.h"
#include "iaimovable.h"

class Archer : public Entity, public IAIMovable
{
public:
    explicit Archer(int health, int xPos, int yPos, int meleeDamage);
    ~Archer() override = default;

    void move(const Cell& cell, int dx, int dy) override;
    int attack(Entity& other) override;
    void takeDamage(int damage) override;
    std::pair<int, int> calculateMovement(int chaserX, int chaserY, int targetX, int targetY) override;
};

#endif // ARCHER_H
