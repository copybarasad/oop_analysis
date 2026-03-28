#pragma once
#include "GameConfig.h"
#include "Position.h"


class Object {
protected:
    GameConfig config;
    Position pos;

public:
    Object(GameConfig &cfg);
    Position &getPosition();
    void setPosition(Position &new_pos);
    void saveTo(SaveFileRAII &out) const;
    void loadFrom(SaveFileRAII &in);
};