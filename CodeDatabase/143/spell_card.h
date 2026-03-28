#ifndef SPELL_CARD_H
#define SPELL_CARD_H

#include <string>
#include "position.h"
#include <vector>
#include <iostream>

class GameController;

class SpellCard {
public:
    virtual ~SpellCard() = default;

    virtual bool cast() = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual bool canCast() const = 0;
    virtual void setTarget(const Position& target) = 0;
    virtual bool isValidTarget(const Position& target) const = 0;
    virtual std::vector<Position> getValidTargets() const = 0;
    virtual int getRange() const = 0;
    virtual int getDamage() const = 0;
    virtual void save(std::ostream& out) const = 0;
    virtual void load(std::istream& in) = 0;

    bool isUsed() const { return used; }
    void markAsUsed() { used = true; }

protected:
    bool used = false;
};

#endif

