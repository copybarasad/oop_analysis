#pragma once
#include <vector>
#include "Position.hpp"

class Field;

class EnemyController {
public:
    static void advanceEnemies(Field& field);

private:
    static std::vector<std::vector<int>> buildDistanceMapToPlayer(const Field& field);
};
