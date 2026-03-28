#ifndef MAPGRID_H
#define MAPGRID_H

#include "Position.h"
#include "CellType.h"
#include <vector>

class MapGrid {
private:
    int width;
    int height;
    std::vector<std::vector<CellType>> cells;

public:
    MapGrid(int width, int height);
    
    MapGrid(const MapGrid& other);
    MapGrid(MapGrid&& other) noexcept;
    
    MapGrid& operator=(const MapGrid& other);
    MapGrid& operator=(MapGrid&& other) noexcept;
    
    int getWidth() const;
    int getHeight() const;
    CellType getCellType(const Position& pos) const;
    
    void generateWalls(int wallCount);
    void generateSlowingCells(int slowingCount);
    void clearGrid();
    
    bool isValidPosition(const Position& pos) const;
    bool canMoveTo(const Position& pos) const;
    bool isReachable(const Position& from, const Position& to) const;
    int countReachableCells(const Position& start) const;
    
    void setCellType(const Position& pos, CellType type);
    
private:
};

#endif
