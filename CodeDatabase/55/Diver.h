#pragma once
#include "GameConfig.h"
#include "Position.h"
#include "Serializable.h"
#include <algorithm>
#include <string>


class Diver {
protected:
    GameConfig config;
    int hp;
    int damage;
    Position pos;
    
public:
    Diver(GameConfig &cfg);
    int getHp();
    void setHp(int amount);
    Position &getPosition();
    void setPosition(Position &new_pos);
    int getDamage();
    void setDamage(int amount);
    bool isAlive() const;
    void takeDamage(int amount);
    bool MoveDiver(Position &new_pos);

    void saveTo(SaveFileRAII &out) const;
    void loadFrom(SaveFileRAII &in);
};