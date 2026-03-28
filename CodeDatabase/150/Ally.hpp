#ifndef ALLY_H
#define ALLY_H

#include "Character.hpp"

class Ally : public Character {
public:
    Ally();
    Ally(int x, int y, int health = 20, int damage = 5);

    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
};

#endif