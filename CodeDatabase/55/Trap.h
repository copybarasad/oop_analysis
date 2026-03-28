#pragma once
#include "Position.h"
#include "GameConfig.h"
#include "Enums.h"
#include "Serializable.h"
#include <string>
class Player;
class Sea;


class Trap{
public:
    virtual Position &getPosition() = 0;
    virtual void setPosition(Position &pos) = 0;
    virtual int getDamage() = 0;
    virtual int getRange() = 0;
    virtual SpellFlag getFlag() = 0;
    virtual std::string getFlagString() const = 0;
    virtual int getCost() = 0;
    virtual bool setTrap(Player &pl, Position &new_pos, Sea &sea) = 0;

    virtual void saveTo(SaveFileRAII &out) const = 0;
    virtual void loadFrom(SaveFileRAII& in) = 0;
};