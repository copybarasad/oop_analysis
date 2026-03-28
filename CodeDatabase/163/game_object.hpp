#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <memory>
#include "cell.hpp"

class Map;

class GameObject {
protected:
    int positionX;
    int positionY;
    Cell* currentCell;
    int healthPoints;
    int maxHealthPoints;
    int movementRange;
    bool isImmovable;

public:
    GameObject(int startX, int startY, Cell& cell, int hp, int range);
    virtual ~GameObject();
    
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    
    GameObject(GameObject&& other) noexcept;
    GameObject& operator=(GameObject&& other) noexcept;
    
    int getPositionX() const;
    int getPositionY() const;
    int getHealthPoints() const;
    int getMaxHealthPoints() const;
    void takeDamage(int damage);
    void heal(int health);
    bool isAlive() const;
    bool moveTo(int newX, int newY, Map& map);
    Cell& getCurrentCell();
    
    virtual void takeTurn(Map& map) = 0;
    virtual void resetTurnFlags() = 0;

protected:
    bool canMoveTo(int x, int y, Map& map) const;
    void updatePosition(int newX, int newY, Cell& newCell);
};

#endif