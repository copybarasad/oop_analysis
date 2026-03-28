#include "SpawnerSystem.h"
#include "GameModel.h"
#include "Field.h"
#include "Spawner.h"
#include <iostream>

void SpawnerSystem::step(GameModel& model) {
    Field& field = model.field();
    std::vector<Coord> spawners;
    for (int r = 0; r < field.rows(); ++r) {
        for (int c = 0; c < field.cols(); ++c) {
            auto ent = field.cellAt({r, c}).getEntity();
            if (ent && dynamic_cast<Spawner*>(ent.get())) spawners.push_back({r, c});
        }
    }

    for (auto bc : spawners) {
        auto ent = field.cellAt(bc).getEntity();
        auto sp = std::dynamic_pointer_cast<Spawner>(ent);
        if (!sp) continue;
        sp->decrementTimer();
        if (sp->readyToSpawn()) {
            bool spawned = field.spawnEnemyNear(bc);
            if (spawned) std::cout << "Spawner spawned an enemy near (" << bc.row << "," << bc.col << ")\n";
            sp->resetTimer();
        }
    }
}
