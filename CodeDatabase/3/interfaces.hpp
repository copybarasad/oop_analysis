#pragma once

class Fightable {
public:
    virtual void minusHealthPoints(int hp) = 0;
    virtual Coordinates getPosition() = 0;
};
