#ifndef SPELL_H
#define SPELL_H

#include "Position.h"

class GameEngine;

class Spell {
public:
    virtual ~Spell() = default;
    virtual bool apply(const Position& targetPosition, GameEngine& game) = 0;
    virtual std::string getName() const = 0;
    virtual int getManaCost() const = 0;
    virtual std::string getDescription() const = 0;
};

#endif