#ifndef UNTITLED_MOVEMENTSYSTEM_H
#define UNTITLED_MOVEMENTSYSTEM_H

#include <tuple>
#include "Coord.h"

class GameModel;
class CombatSystem;

class MovementSystem {
public:
    MovementSystem(GameModel& model, CombatSystem& combat);

    std::tuple<bool,bool,bool,bool,bool> moveEntity(Coord from, Coord to);

private:
    GameModel& model_;
    CombatSystem& combat_;
};

#endif // UNTITLED_MOVEMENTSYSTEM_H

