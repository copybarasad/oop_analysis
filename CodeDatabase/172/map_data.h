#pragma once

#include "cell.h"
#include "pos.h"

#include <vector>

namespace game {
    struct MapData {
        int width{};
        int height{};

        std::vector<std::vector<Cell>> cells;

        Pos playerStart{};
        Pos enemyBuilding;
        Pos enemyTower;
    };
}