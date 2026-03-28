#include "game_map.h"

GameMap::GameMap(int map_width, int map_height) {
    if (map_width < MIN_MAP_SIZE) 
        width = MIN_MAP_SIZE;
    else if (map_width > MAX_MAP_SIZE) 
        width = MAX_MAP_SIZE;
    else 
        width = map_width;
    
    if (map_height < MIN_MAP_SIZE) 
        height = MIN_MAP_SIZE;
    else if (map_height > MAX_MAP_SIZE) 
        height = MAX_MAP_SIZE;
    else 
        height = map_height;
    grid.resize(height, std::vector<MapCell>(width));
}

GameMap::GameMap(const GameMap& other) 
    : width(other.width), height(other.height) {
    grid.resize(height);
    for (int i = 0; i < height; ++i) {
        grid[i] = other.grid[i];
    }
}


GameMap::GameMap(GameMap&& other) 
    : width(other.width), height(other.height), grid(std::move(other.grid)) {
    other.width = 0;
    other.height = 0;
}


GameMap& GameMap::operator=(const GameMap& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        
        grid.resize(height);
        for (int i = 0; i < height; ++i) {
            grid[i] = other.grid[i];
        }
    }
    return *this;
}

GameMap& GameMap::operator=(GameMap&& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
        
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

bool GameMap::isPositionValid(const Position& pos) const {
    return pos.getX() >= 0 && pos.getX() < width && 
           pos.getY() >= 0 && pos.getY() < height;
}

MapCell& GameMap::getCell(const Position& pos) {
    return grid[pos.getY()][pos.getX()];
}

const MapCell& GameMap::getCell(const Position& pos) const {
    return grid[pos.getY()][pos.getX()];
}

int GameMap::getWidth() const { return width; }
int GameMap::getHeight() const { return height; }