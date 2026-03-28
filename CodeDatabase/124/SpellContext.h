#ifndef SPELL_CONTEXT_H
#define SPELL_CONTEXT_H
 
#include "player.h"
#include "gamefield.h"
#include "enemy.h"

struct SpellContext {
    Player& player;
    GameField& field;
    Enemy& enemy;
    int targetX;
    int targetY;
    
    SpellContext(Player& p, GameField& f, Enemy& e, int x, int y) 
        : player(p), field(f), enemy(e), targetX(x), targetY(y) {}
};

#endif