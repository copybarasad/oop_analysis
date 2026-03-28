#ifndef RANGEDATTACKLOGIC_H
#define RANGEDATTACKLOGIC_H

#include <cmath>

class GameField;

class RangedAttackLogic {
public:
    static bool isInRange(int sourceX, int sourceY, int targetX, int targetY, int range) {
        int distance = std::abs(targetX - sourceX) + std::abs(targetY - sourceY);
        return distance <= range;
    }

    static bool isPathClear(int startX, int startY, int targetX, int targetY, const GameField& field);
};

#endif // RANGEDATTACKLOGIC_H