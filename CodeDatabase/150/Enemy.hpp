#ifndef ENEMY_H
#define ENEMY_H

#include "Character.hpp"

class Enemy : public Character {
public:
    Enemy();
    Enemy(int x, int y, int health = 30, int damage = 15);

    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
};

#endif