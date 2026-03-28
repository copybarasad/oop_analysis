#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "field.h"
#include "freezy_system.h"

class MovementSystem{

public:
    static bool moves_game_object(GameObject& object, int new_x, int new_y, Field& field);
};

#endif