#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include "tile.hpp"
#include "position.hpp"

class Map {
private:
    int width, height;
    std::vector<std::vector<Tile>> tiles;

public:
    Map(int w = 10, int h = 10);

    Map(const Map& other_map);
    Map(Map&& moved_map) noexcept;

    Map& operator=(const Map& other_map);
    Map& operator=(Map&& moved_map) noexcept;

    int getWidth() const;
    int getHeight() const;

    std::vector<Position> freeCells() const;
    const Tile& getTile(int x, int y) const;
    void setTile(int x, int y, TileType type);
    
};

#endif