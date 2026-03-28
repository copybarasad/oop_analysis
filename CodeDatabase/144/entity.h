#ifndef ENTITY_H
#define ENTITY_H

class Game;

class Entity {
protected:
    int health;
    int x;
    int y;

public:
    Entity(int health = 100, int x = -1, int y = -1);
    virtual ~Entity() = default;
    
    virtual void update(Game& game) = 0;
    
    int getHealth() const;
    int getX() const;
    int getY() const;
    void setPosition(int newX, int newY);
    void takeDamage(int damage);
    bool isAlive() const;
};

#endif