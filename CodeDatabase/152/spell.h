#ifndef GAME_SPELL_H_
#define GAME_SPELL_H_
class Game;
#include "game_field.h"
#include "player.h"
#include "enemy.h"
class Spell {
public:
    virtual ~Spell() = default;
    virtual void use(Game& game, GameField& field, Player& player, int target_row, int target_col) = 0;
    virtual const char* name() const = 0;
    virtual int getType() const = 0;
    virtual int getDamage() const = 0;
    virtual int getRadius() const = 0;
};
#endif