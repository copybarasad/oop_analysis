#ifndef IMAKETURNH
#define IMAKETURNH
#include "coords.h"
class map;

class iMakeTurn{
public:
    virtual void environmentInteraction(map*, coords) = 0; //fair?
    virtual void makeTurn(map*) = 0;
};
#endif