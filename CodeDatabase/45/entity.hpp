#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <string>

class Map;

class Entity {
protected:
    int hp;
    int damage;
    bool canMove;

public:
    Entity(int health, int damageValue);
    virtual ~Entity() = default;

    int getHp() const;
    int getDamage() const;
    void takeDamage(int damageAmount);
    bool getCanMove() const;
    void setCanMove(bool can);
    virtual void step(Map* map, int x, int y) = 0;
    virtual std::string getStatus() const = 0;

};

#endif