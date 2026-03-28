#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

class Map;

class Player : public Entity {
private:
    int stars;
public:
    Player(int x, int y, int hp_ = 10, int dmg = 4);
    Player();

    int getStars() const;
    void addStars(int val);
    void setStars(int val);
};

#endif