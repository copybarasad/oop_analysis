#ifndef FREEZY_SYSTEM_H
#define FREEZY_SYSTEM_H

#include "field.h"
#include "game_objects.h"

class Freezy_System{
public:
    static void freezyObject(GameObject& enemy);
    static void freezyReduceObject(GameObject& enemy);
    static bool isFrozen(GameObject& object);

};



#endif