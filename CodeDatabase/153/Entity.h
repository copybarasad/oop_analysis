#ifndef ENTITY_H
#define ENTITY_H

class Entity {
protected:
    int health;
    int damage;

public:
    Entity(int startHealth, int startDamage);
    ~Entity() = default;

    int getHealth() const;
    int getDamage() const;
    void takeDamage(int amount);
    bool isAlive() const;
};

#endif
