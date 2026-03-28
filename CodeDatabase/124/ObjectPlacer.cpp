#include "ObjectPlacer.h"

ObjectPlacer::ObjectPlacer(MapManager& mapManager) : map(mapManager) {}

bool ObjectPlacer::placePlayer(int x, int y) {
    if (!map.isValidPosition(x, y) || !map.getCell(x, y).getIsPassable()) 
        return false;
    map.getCell(x, y).setHasPlayer(true);
    return true;
}

bool ObjectPlacer::placeEnemy(int x, int y) {
    if (!map.isValidPosition(x, y) || !map.getCell(x, y).getIsPassable()) 
        return false;
    map.getCell(x, y).setHasEnemy(true);
    return true;
}

bool ObjectPlacer::moveObject(int fromX, int fromY, int toX, int toY) {
    if (!map.isValidPosition(fromX, fromY) || !map.isValidPosition(toX, toY)) 
        return false;
    
    Cell& from = map.getCell(fromX, fromY);
    Cell& to = map.getCell(toX, toY);
    
    if (!to.getIsPassable() || to.getIsOccupied()) 
        return false;
    
    if (from.getHasPlayer()) {
        from.setHasPlayer(false);
        to.setHasPlayer(true);
    } else if (from.getHasEnemy()) {
        from.setHasEnemy(false);
        to.setHasEnemy(true);
    } else return false;
    
    return true;
}

bool ObjectPlacer::findPlayerPosition(int& x, int& y) const {
    for (int i = 0; i < map.getHeight(); i++) {
        for (int j = 0; j < map.getWidth(); j++) {
            if (map.getCell(j, i).getHasPlayer()) {
                x = j;
                y = i;
                return true;
            }
        }
    }
    return false;
}

bool ObjectPlacer::findEnemyPosition(int& x, int& y) const {
    for (int i = 0; i < map.getHeight(); i++) {
        for (int j = 0; j < map.getWidth(); j++) {
            if (map.getCell(j, i).getHasEnemy()) {
                x = j;
                y = i;
                return true;
            }
        }
    }
    return false;
}