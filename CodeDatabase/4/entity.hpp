#pragma once

class Entity {
public:
    int id;
    int health;
    int damage;

    Entity(int id, int health, int damage);
    virtual ~Entity() = default;
    virtual bool alive() const;
    virtual void get_damage(int dmg);
};