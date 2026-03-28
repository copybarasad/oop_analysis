#ifndef ALLY_MOVEMENT_S_H
#define ALLY_MOVEMENT_S_H

#include "../BaseMovementService/MovementService.hpp"

class AllyMovementService : public MovementService {
    public:
    MoveResult move (Field& field, Cell& currentCell, size_t x, size_t y) override;

    private:
    std::pair<int, int> calculateAllyMove (Field& field, std::pair<size_t, size_t> allyPosition, std::pair<size_t, size_t> playerPosition) const;
    std::pair<int, int> findEnemy (Field& field, std::pair<size_t, size_t> allyPosition) const;
    bool checkEnemy (Field& field, int x, int y) const;
    std::pair<int, int> findAlternativeMove (const Field& field, std::pair<size_t, size_t> enemyPosition) const;
};

#endif