#include "TrapActor.h"
#include "GameBoard.h"
#include "Foe.h"
#include <iostream>

TrapActor::TrapActor(const Location& pos, int trapDamage)
    : GameActor(1, trapDamage, pos), damage(trapDamage) {
}

void TrapActor::performMove(MoveDirection dir, GameBoard& board) {
    
}

void TrapActor::handleCollision(GameActor* other) {
    Foe* enemy = dynamic_cast<Foe*>(other);
    if (enemy && enemy->isAlive()) {
        triggerTrap(enemy);
    }
}

void TrapActor::triggerTrap(GameActor* victim) {
    if (isAlive()) {
        victim->receiveDamage(damage);
        std::cout << "Trap triggered! " << damage << " damage dealt to enemy.\n";
        receiveDamage(1); 
    }
}