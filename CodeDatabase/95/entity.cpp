#include "entity.hpp"

Entity::Entity(int x, int y, Faction f,int hp_, int dmg) 
    : pos{x, y}, faction(f), hp(hp_), damage(dmg) {}

Position Entity::getPos() const {
    return pos;
}

Faction Entity::getFaction() const {
    return faction;
}

bool Entity::isAlive() const {
    return hp > 0;
}

int Entity::getHP() const {
    return hp;
}

int Entity::getDamage() const {
    return damage;
}

void Entity::setPos(int x, int y) { 
    pos.x = x; 
    pos.y = y; 
}

void Entity::setFaction(Faction f) { 
    faction = f; 
}

void Entity::setHP(int health) { 
    hp = health; 
}

void Entity::setDamage(int dmg) { 
    damage = dmg; 
}

void Entity::takeDamage(int dmg){
    hp -= dmg;

    if (hp < 0) {
        hp = 0;
    }
}
