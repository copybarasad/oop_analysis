#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "game_world/position.hpp"

enum class Faction {
    Neutral,
    Ally,
    Enemy,
};

class Map;

class Entity {
protected:
    Position pos;
    Faction faction;
    int hp;
    int damage;
    
public:
    Entity(int x, int y, Faction f, int hp, int dmg);
    virtual ~Entity() = default;

    Position getPos() const;
    void setPos(int x, int y);

    Faction getFaction() const;
    void setFaction(Faction f);

    bool isAlive() const;

    int getHP() const;
    void setHP(int health);

    int getDamage() const;
    void setDamage(int dmg);

    virtual void takeDamage(int dmg);

};

#endif