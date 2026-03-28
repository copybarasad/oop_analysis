#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "cell.h"

class MapManager {
private:
    int width, height;
    Cell** cells;

public:
    MapManager(int w, int h);
    ~MapManager();
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isValidPosition(int x, int y) const;
    Cell& getCell(int x, int y) const;
    
    void addObstacles(int count);
    void addObstacle(int x, int y);
    bool isCellEmpty(int x, int y) const;
    void clearCell(int x, int y);

private:
    void allocateCells();
    void deallocateCells();
    static void initRandom();
};

#endif