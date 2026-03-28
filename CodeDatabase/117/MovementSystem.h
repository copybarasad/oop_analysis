#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../Characters/Character.h"
#include "../Field/Field.h"

#include <cstdlib>

class MovementSystem{
public:
    bool move(Character& object, std::pair<int, int> point, Field& field);
    bool step(Character& object, int dx, int dy, Field& field);
    void stepToTar(Character& object, Character& target, Field& field);
    void deleteFromField(Character& object, Field& field);
};

#endif