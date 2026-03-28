#include "Foe.h"
#include "GameBoard.h"
#include "Hero.h"
#include <iostream>
#include <cmath>

Foe::Foe(int hitpoints, int attack, const Location& pos)
    : GameActor(hitpoints, attack, pos) {
}

void Foe::performMove(MoveDirection dir, GameBoard& board) {
    Location newPos = position;
    
    switch (dir) {
        case MoveDirection::North: newPos.row--; break;
        case MoveDirection::South: newPos.row++; break;
        case MoveDirection::West:  newPos.col--; break;
        case MoveDirection::East:  newPos.col++; break;
    }
    
    if (board.isMoveValid(position, newPos, this)) {
        board.moveActor(position, newPos, this);
        position = newPos;
        std::cout << "Foe moved to (" << newPos.row << "," << newPos.col << ")\n";
    } else {
        GameActor* other = board.getActorAt(newPos);
        if (other) {
            handleCollision(other);
        }
    }
}

void Foe::handleCollision(GameActor* other) {
    Hero* hero = dynamic_cast<Hero*>(other);
    if (hero) {
        attackTarget(hero);
    }
}

MoveDirection Foe::calculateBestMove(const GameBoard& board, const Location& target) const {
    int diffRow = static_cast<int>(target.row) - static_cast<int>(position.row);
    int diffCol = static_cast<int>(target.col) - static_cast<int>(position.col);
    
    if (std::abs(diffCol) > std::abs(diffRow)) {
        return (diffCol > 0) ? MoveDirection::East : MoveDirection::West;
    } else {
        return (diffRow > 0) ? MoveDirection::South : MoveDirection::North;
    }
}

void Foe::attackTarget(GameActor* target) {
    target->receiveDamage(getAttackPower());
    displayAttack();
}

void Foe::displayAttack() const {
    std::cout << "Foe attacked! Damage: " << getAttackPower() << "\n";
}