#ifndef ISPELL_H
#define ISPELL_H

#include <memory>

class Player;
class GameField;

class ISpell {
public:
    virtual ~ISpell() = default;
    virtual bool use(Player& player, GameField& field, int targetX, int targetY) = 0;
    virtual const char* name() const = 0;
};

#endif // ISPELL_H
