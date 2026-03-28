#ifndef CASTLEH
#define CASTLEH
#include "../abstractNinterfaces/destroyable.h"
#include "../abstractNinterfaces/iMakeTurn.h"
class castle : public destroyable, private iMakeTurn{
    size_t spawnTimer;
public:
    void environmentInteraction(map*, coords);
    void makeTurn(map*);
    void setSpawnTimer(size_t);
    size_t getSpawnTimer();
    castle(int, size_t);
    castle(int);
    castle();
};
#endif