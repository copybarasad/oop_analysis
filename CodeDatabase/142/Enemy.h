#pragma once
#include "Entity.h"

// Forward declarations
class Field;

class Enemy : public Entity {
private:
    int damage;

public:
    Enemy(int startX, int startY, int h = 30, int d = 5);
    
    Enemy(const Enemy& other);
    Enemy& operator=(const Enemy& other);
    Enemy(Enemy&& other) noexcept;
    Enemy& operator=(Enemy&& other) noexcept;

    bool canMove(int dx, int dy, const Field& field) const;
    bool move(int dx, int dy, Field& field);
    bool attack(int targetX, int targetY, Field& field);
    
    int getDamage() const override;
    char getDisplayChar() const override;
};
