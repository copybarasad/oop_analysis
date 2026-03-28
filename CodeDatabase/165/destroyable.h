#ifndef DESTROYABLEH
#define DESTROYABLEH
#include <stddef.h>
#include "./coords.h"
class destroyable{
    coords mapPosition;
    size_t id;
    int HP;
public:
    void setHP(int);
    void setMapPosition(size_t, size_t);
    void setId(size_t);
    int getHP();
    coords getMapPosition();
    size_t getId();
};
#endif