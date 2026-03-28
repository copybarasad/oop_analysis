//
// Created by bob on 10/4/25.
//

#ifndef GAME_TERM_CHARACTER_H
#define GAME_TERM_CHARACTER_H
#include <string>

#include "Entity.h"
#define DEFAULT_DAMAGE 20
#define DEFAULT_HEALTH 100
#define DEFAULT_POSITION {0, 0}

class Character: public Entity{


protected:

    int validateHealth(const int &value) const;

    int validateDamage(const int &value) const;

    int health;
    int damage;


public:

    explicit Character(const int & damage = DEFAULT_DAMAGE, const Position & position = DEFAULT_POSITION, const int & health = DEFAULT_HEALTH);

    void setDamage(const int & damage);

    void setHealth(const int & health);

    int getDamage() const;

    int getHealth() const;

    bool isAlive() const;

    void applyDamage(const int & damage);
};


#endif //GAME_TERM_CHARACTER_H