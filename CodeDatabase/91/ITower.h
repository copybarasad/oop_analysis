#ifndef ITOWER_H
#define ITOWER_H

#include "../Position.h"

class Game;

class ITower {
    public:
        virtual ~ITower() = default;

        virtual void update(Game& game) = 0;
        virtual bool canAttack(const Game& game) const = 0;
        virtual void attack(Game& game) = 0;

        virtual void takeDamage(int damage) = 0;
        virtual bool isAlive() const = 0;
        virtual int getHealth() const = 0;

        virtual Position getPosition() const = 0;
        virtual int getRange() const = 0;
        virtual bool isActive() const = 0;
        virtual int getCooldown() const = 0;
        virtual char getSymbol() const = 0;

        virtual void applyLevelBonus(int level) = 0;
};

#endif