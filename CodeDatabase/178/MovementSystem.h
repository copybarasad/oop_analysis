#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "Position.h"

class MovementSystem {
private:
    GameField& field;
    
public:
    MovementSystem(GameField& gameField);
    Position calculateNewPosition(const Position &current, const Position &direction) const;
    bool isValidMove(const Position &newPosition) const;
    void movePlayer(Player &player, const Position &newPosition);
    void moveEnemy(Enemy &enemy, const Position &newPosition);
};

#endif