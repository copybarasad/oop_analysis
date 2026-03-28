#ifndef PAWNH
#define PAWNH
#include "destroyable.h"
#include "iMakeTurn.h"
class pawn : public destroyable, protected iMakeTurn{ //remember the crash question
    size_t attackDmg;
public:
    void setAttackDmg(size_t);
    size_t getAttackDmg();
};
#endif