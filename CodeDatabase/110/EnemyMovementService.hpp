#ifndef ENEMY_MOVEMENT_S_H
#define ENEMY_MOVEMENT_S_H

#include "../BaseMovementService/MovementService.hpp"

class EnemyMovementService : public MovementService {
    public:
    MoveResult move (Field& field, Cell& currentCell, size_t x, size_t y) override;

    private:
    std::pair<int, int> calculateEnemyMove (std::pair<size_t, size_t> enemyPosition, std::pair<size_t, size_t> playerPosition) const;
    std::pair<int, int> findAlternativeMove (const Field& field, std::pair<size_t, size_t> enemyPosition) const;
};

#endif