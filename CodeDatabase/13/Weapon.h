#ifndef GAME_WEAPON_H
#define GAME_WEAPON_H

#include <string>
class Object;
class Entity;


class Weapon {
public:
    const std::string name_;
    const int damage_;
    const int range_;
    Entity *owner_;

    explicit Weapon(Entity *owner, const std::string &name = "Weapon", const int damage = 1, const int range = 1)
        : name_(name), damage_(damage), range_(range), owner_(owner) {
    };

    int hit(Object *target) const;
};


#endif
