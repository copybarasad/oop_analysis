#include "BuildingSystem.h"
#include "GameModel.h"
#include "Field.h"
#include "EnemyBuilding.h"
#include <iostream>

void BuildingSystem::step(GameModel& model) {
    Field& field = model.field();
    std::vector<Coord> buildings;
    for (int r = 0; r < field.rows(); ++r) {
        for (int c = 0; c < field.cols(); ++c) {
            auto ent = field.cellAt({r,c}).getEntity();
            if (ent && dynamic_cast<EnemyBuilding*>(ent.get())) buildings.push_back({r,c});
        }
    }

    for (auto bc : buildings) {
        auto ent = field.cellAt(bc).getEntity();
        auto bptr = std::dynamic_pointer_cast<EnemyBuilding>(ent);
        if (!bptr) continue;
        bptr->decrementTimer();
        if (bptr->tryAttackPlayer(field, bc)) {
            std::cout << "Building at (" << bc.row << "," << bc.col << ") attacked player\n";
        }
    }
}
