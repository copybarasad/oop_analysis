#pragma once
#include "Board.h"
#include "MovableEntity.h"
#include "CombatController.h"

// Контроллер отвечает за движение сущностей по полю.
class MovementController {
private:
    Board& board;
    CombatController& combat;

public:
    MovementController(Board& b, CombatController& c);

    // Попытка передвинуть сущность на dx, dy.
    bool move(MovableEntity& entity, int dx, int dy);

    // Проверка, можно ли войти в клетку
    bool canMoveTo(int x, int y) const;
};
