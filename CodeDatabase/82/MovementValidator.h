#ifndef MOVEMENTVALIDATOR_H
#define MOVEMENTVALIDATOR_H

#include "Position.h"
#include <vector>

class GameField;
class Enemy;
class Player;

class MovementValidator {
public:
    static bool isValidPosition(const Position& position, const GameField& field);
    static bool isCellPassable(const Position& position, const GameField& field);
    static bool hasEnemyAtPosition(const Position& position, const std::vector<Enemy>& enemies);
    static bool hasPlayerAtPosition(const Position& position, const Player& player);
    static bool arePositionsAdjacent(const Position& pos1, const Position& pos2);
    
private:
    MovementValidator() = delete;
};

#endif