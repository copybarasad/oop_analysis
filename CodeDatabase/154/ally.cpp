#include "ally.h"
#include "field.h"
#include <cmath>
#include <iostream>

Ally::Ally(int id, int health, int damage) : Entity(id, health), damage_(damage) {}

EntityType Ally::getType() const { return EntityType::Ally; }

int Ally::getDamage() const { 
    return damage_; 
}

std::pair<int,int> Ally::chooseStep(const Field& field, int allyRow, int allyCol) const {
    int closestEnemyRow = -1;
    int closestEnemyCol = -1;
    int minDist = 1000000;

    for (int r = 0; r < field.rows(); ++r) {
        for (int c = 0; c < field.cols(); ++c) {
            int entityId = field.getOccupantIdAt(r, c);
            if (entityId != -1 && field.isEnemy(entityId)) {
                int dist = field.manhattanDistance(allyRow, allyCol, r, c);
                if (dist < minDist) {
                    minDist = dist;
                    closestEnemyRow = r;
                    closestEnemyCol = c;
                }
            }
        }
    }

    if (closestEnemyRow != -1) {
        int dr = closestEnemyRow - allyRow;
        int dc = closestEnemyCol - allyCol;
        
        if (std::abs(dr) >= std::abs(dc)) {
            return {dr > 0 ? 1 : (dr < 0 ? -1 : 0), 0};
        } else {
            return {0, dc > 0 ? 1 : (dc < 0 ? -1 : 0)};
        }
    }

    return {0, 0};
}