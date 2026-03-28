#ifndef SPELLCARD_H
#define SPELLCARD_H

#include <string>

class GameField;
class Player;

class SpellCard {
public:
    virtual ~SpellCard() = default;
    
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getManaCost() const = 0;
    virtual int getRange() const = 0;
    virtual bool canCast(int casterX, int casterY, int targetX, int targetY, GameField* field) const = 0;
    virtual bool cast(Player& caster, int targetX, int targetY, GameField* field) = 0; // Changed to bool
    virtual SpellCard* clone() const = 0;
};

#endif