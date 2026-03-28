#pragma once
#include <ScreenSize.hpp>

class Entity {
public:
    enum class Type {
        ENTITY,
        FLOOR,
        WALL,
        PLAYER,
        ENEMY,
        ENEMY_BASE,
        SPIKED_TRAP,
        PLAYER_TRAP,
        ARCHER_TOWER,
        DECOY_ALLY,
    };
    
    Entity(int x, int y, Type type);

    int getY() const;
    int getX() const;
    void setX(int);
    void setY(int);
    virtual void draw() = 0;
    virtual int getHealth() const;
    Entity::Type getType() const;
    virtual ~Entity() = default;
    
protected:
    int x;
    int y;
    Type type;
};