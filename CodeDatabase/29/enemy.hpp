#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "actor.hpp"

class Enemy : public Actor {
public:
    explicit Enemy(int hp, int damage, Point pos);
    ~Enemy();

    bool StepOrHit(const Board& board, const Point& player_pos, int& out_damage_to_player);
};

#endif