#pragma once
#include "GameActor.h"
#include "MoveDirection.h"

enum class MoveDirection;

class Foe : public GameActor {
public:
    Foe(int hitpoints, int attack, const Location& pos);
    
    void performMove(MoveDirection dir, GameBoard& board) override;
    void handleCollision(GameActor* other) override;
    
    MoveDirection calculateBestMove(const GameBoard& board, const Location& target) const;

private:
    void attackTarget(GameActor* target);
    void displayAttack() const;
};