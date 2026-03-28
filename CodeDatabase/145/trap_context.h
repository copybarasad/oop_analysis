#ifndef TRAP_CONTEXT_H
#define TRAP_CONTEXT_H

class Player;
class GameField;

struct TrapContext {
    Player& player;
    GameField& field;
    int targetX;
    int targetY;
};

#endif