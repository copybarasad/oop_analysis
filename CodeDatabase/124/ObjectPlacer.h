#ifndef OBJECT_PLACER_H
#define OBJECT_PLACER_H

#include "mapmanager.h"

class ObjectPlacer {
private:
    MapManager& map;

public:
    ObjectPlacer(MapManager& mapManager);
    
    bool placePlayer(int x, int y);
    bool placeEnemy(int x, int y);
    bool moveObject(int fromX, int fromY, int toX, int toY);
    bool findPlayerPosition(int& x, int& y) const;
    bool findEnemyPosition(int& x, int& y) const;
};

#endif