#ifndef ISPELL_H
#define ISPELL_H

#include <string>

class GameField;
class Player;

enum SpellID {
    MEGASHOTGUN,
    BULLETARAIN, 
    CHEAPWHISKEY,
    GHOSTOFJHONNY,
    UPDATESPELL
};

class ISpell {
public:
    virtual ~ISpell() = default;
    
    virtual bool use(int x, int y, GameField& field, Player& player) = 0;
    virtual std::string getName() const = 0;
    virtual int getLevel() const = 0;
};

#endif