#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "position.h"
#include "map_cell.h"
#include <vector>


class GameMap {
private:
    int width;
    int height;
    std::vector<std::vector<MapCell>> grid;

    const int MIN_MAP_SIZE = 10;
    const int MAX_MAP_SIZE = 25;

public:
    GameMap(int map_width, int map_height);
    GameMap(const GameMap& other);
    GameMap(GameMap&& other);
    

    GameMap& operator=(const GameMap& other);
    GameMap& operator=(GameMap&& other);
    
    bool isPositionValid(const Position& pos) const;
    MapCell& getCell(const Position& pos);
    const MapCell& getCell(const Position& pos) const;
    
    int getWidth() const;
    int getHeight() const;

    ~GameMap() = default; 
};

#endif