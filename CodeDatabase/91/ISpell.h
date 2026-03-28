#ifndef ISPELL_H
#define ISPELL_H

#include "../Position.h"
#include <string>

class Game;

class ISpell {
    public:
        virtual ~ISpell() = default;

        virtual bool cast(Game& game, const Position& target) const = 0;
        virtual bool canTarget(const Game& game, const Position& casterPos, const Position& target) const = 0;

        // === Info about spell ===
        virtual std::string getName() const = 0;
        virtual std::string getDescription() const = 0;
        virtual int getManaCost() const = 0;
        virtual int getRange() const = 0;
        virtual int getDamage() const = 0;
        virtual char getSymbol() const = 0;

        virtual void upgrade() = 0;
};

#endif