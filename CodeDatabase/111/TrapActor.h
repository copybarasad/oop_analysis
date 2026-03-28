#pragma once
#include "GameActor.h"

class TrapActor : public GameActor {
private:
    int damage;

public:
    TrapActor(const Location& pos, int trapDamage);
    
    void performMove(MoveDirection dir, GameBoard& board) override;
    void handleCollision(GameActor* other) override;
    
    void triggerTrap(GameActor* victim);
    bool shouldRemove() const { return !isAlive(); }
};