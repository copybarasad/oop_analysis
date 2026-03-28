#ifndef PLAYER_MOVEMENT_S_H
#define PLAYER_MOVEMENT_S_H

#include "../BaseMovementService/MovementService.hpp"

class PlayerMovementService : public MovementService {
    public:
    PlayerMovementService () = default;
    MoveResult move (Field& field, Cell& currentCell, size_t x, size_t y) override;

    private:
    bool checkCloseFight (MoveResult& result, Cell& currentCell, Cell& goalCell) const;
    bool checkRangedFight (Field& field, MoveResult& result, Cell& newCell) const;
    bool checkDirection (Field& field, int x, int y) const;
};

#endif