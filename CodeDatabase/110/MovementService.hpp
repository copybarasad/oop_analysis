#ifndef MOVEMENT_S_H
#define MOVEMENT_S_H

#include <string>

#include "../../../Field/Field/Field.hpp"
#include "../../../Entities/Player/Player.hpp"
#include "../../../Entities/Enemies/Enemy.hpp"
#include "../../../Entities/Ally/Ally.hpp"

class MovementService {
    public:
    struct MoveResult {
        bool success;
        bool fight;
        Cell* opponent;
        bool slowTriggered;
        Cell* newCell;

        MoveResult () : success(true), fight(false), opponent(nullptr), slowTriggered(false), newCell(nullptr) {}
    };

    virtual MoveResult move (Field& field, Cell& currentCell, size_t x, size_t y) = 0;

    protected:
    MovementService () = default;

    bool moveIsAble (const Field& field, int x, int y) const;
    bool isSlow (const Field& field, size_t x, size_t y) const;
    bool isBlocked (const Field& field, size_t x, size_t y) const;
    void swapCells (Cell& current, Cell& next);
};

#endif