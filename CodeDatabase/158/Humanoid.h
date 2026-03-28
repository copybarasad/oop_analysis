#pragma once

#include <string>
#include <sstream>
#include "Core/Coordinates.h"
#include "Core/Cooldown.h"

enum class ATKtype 
{
    melee,
    ranged
};

enum class Direction 
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    KNOWHERE
};

class Humanoid:public Cooldown 
{
protected:
    std::string Body;
    int HP;
    int maxHP;
    int ATK;
    int Speed;
    bool IsPlayer;
    bool IsAlive;
    ATKtype type;
    Coordinates XY_coord;
    Direction where;

public:
    Humanoid(bool IsPlayer, bool IsAlive, ATKtype type, std::string Body, int HP, int ATK, int X_coord, int Y_coord, int Speed);
    
    int getX_coord();
    int getY_coord();
    void setXY_coord(int new_X_coord, int new_Y_coord);
    void take_damage(int damage);
    int getSpeed() const;
    void setSpeed(int newSpeed);
    bool checkPulse() const;
    int getHP() const;
    void setHP(int newHP);
    int getMaxHP() const;
    void setMaxHP(int newMaxHP);
    void healFull();
    int getAttackDamage() const;
    Coordinates getCoordinates() const;
    
    std::string serialize() const;
    void deserialize(const std::string& data);
};