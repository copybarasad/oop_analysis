#pragma once
#include "Field.hpp"
class Field;

class NestController {
public:
    static void processNests(Field& field);
private:
    static bool trySpawnEnemyAdjacent(Field& f, Field::NestEntry& n);
};
