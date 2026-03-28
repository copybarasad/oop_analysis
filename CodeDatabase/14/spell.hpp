#ifndef SPELL_H
#define SPELL_H

#include <string>

class Player;
class GameBoard;

class Spell{
    public:
        virtual ~Spell() = default;

        virtual bool cast(GameBoard& board, Player& player, int targetX, int targetY) = 0;

        virtual std::string getName() const = 0;
        virtual std::string getDescription() const = 0;
        virtual double getRange() const = 0;
        virtual bool needsTarget() const {
            return true;
        }
        virtual int getTypeId() const = 0;

};

#endif