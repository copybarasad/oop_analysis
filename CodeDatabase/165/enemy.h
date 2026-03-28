#ifndef ENEMYH
#define ENEMYH
#include "../abstractNinterfaces/pawn.h"
class enemy : public pawn{
public:
    void environmentInteraction(map*, coords);
    void makeTurn(map*);
    enemy(int, size_t);
    enemy(int);
    enemy();
};
#endif