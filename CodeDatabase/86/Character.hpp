#pragma once
#include "Logger.hpp"
#include "Position.hpp"
#include <cmath>
#include <string>
 #include <iostream>


enum class attackType { melee, range };

class Character {
protected:
    int hp_ = 100;
    int damage_ = 10;
    Position pos_;
    int stars_ = 25;
    attackType attack;
    Logger& logger_;

public:
    Character(int hp, int damage, Position pos, Logger& logger);
    virtual ~Character() = default;

    virtual bool isAlive() const;
    virtual void takeDamage(int damage);

    int distanceTo(const Character& other) const;

    void melee_attack(Character& other);
    void range_attack(Character& other);

    void setPos(Position pos);
    int getHp() const;
    int getDamage() const;
    Position getPos() const;
    attackType getAttackType() const;


    virtual void save(std::ostream& os) const;
    virtual void load(std::istream& is);
};
