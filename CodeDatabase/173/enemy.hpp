#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "creature.hpp"

class Enemy : public Creature {
  public:
    Enemy(int hp, int maxHP);
    Enemy(const Enemy &enemy);
    Enemy(bytedump bytes);

    std::string chr() override;
    Color baseColor() override;
};

#endif /* ENEMY_HPP */
