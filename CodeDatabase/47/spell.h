#ifndef SPELL_H
#define SPELL_H

#include <string>

class Game;

class Spell {
public:
    virtual ~Spell() = default;

    virtual bool cast(Game& game, int targetX, int targetY) = 0;

    virtual bool isValidTarget(const Game& game, int targetX, int targetY) const = 0;

    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getManaCost() const = 0;
    virtual int getRange() const = 0;
    virtual int getDamage() const = 0;
    virtual void increaseDamage(int amount) = 0;
};

#endif