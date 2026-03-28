
#ifndef SPAWN_SYSTEM_H
#define SPAWN_SYSTEM_H

#include "building.h"
#include "field.h"
#include "enemy.h"

class SpawnSystem{
public:
    static bool spawn_enemy(Building& build, Field& field);
};

#endif