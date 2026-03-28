#ifndef ALLY_HPP
#define ALLY_HPP

#include "creature.hpp"

class Ally : public Creature {
  public:
    Ally(int hp, int maxHP);
    Ally(const Ally &ally);
    Ally(bytedump bytes);

    std::string chr() override;
    Color baseColor() override;
};

#endif /* ALLY_HPP */
