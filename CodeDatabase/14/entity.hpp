#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>

class GameBoard;
class Player;

enum class EntityType {
    PLAYER,
    ENEMY,
    BUILDING,
    TOWER,
    ALLY,
    TRAP
};

class Entity {
protected:
    int x_coordinate;
    int y_coordinate;

public:
    Entity(int x, int y);
    virtual ~Entity() = default;

    int getX() const;
    int getY() const;
    void setPosition(int x, int y);

    virtual void takeDamage(int dmg);
    virtual void onDeath(Player* player);
    virtual bool isEnemy() const;
    virtual bool isTrap() const;
    virtual int getHealth() const;
    virtual void onSteppedOn(Entity* activator);

    virtual EntityType getEntityType() const = 0;

    virtual void removeFromBoard(GameBoard& board) = 0;

    virtual void save(std::ostream& out) const;
    virtual void load(std::istream& in);
};

#endif