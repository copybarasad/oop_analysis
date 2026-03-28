#ifndef SPELL_H
#define SPELL_H

#include <string>

class GameField;
class Player;

class Spell {
public:
    virtual ~Spell() = default;
    virtual std::string name() const = 0;
    virtual bool canTargetCell() const = 0;
    virtual bool cast(GameField& field, Player& player, int targetX, int targetY) = 0;
};

#endif 

