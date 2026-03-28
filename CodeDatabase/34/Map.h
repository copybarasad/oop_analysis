#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <vector>
#include <memory>
#include <iostream>
#include "../utils/Point.h"
#include "MapCell.h"

namespace Game {

class Map {
public:
    Map(int width, int height);
    Map(std::istream& is); // Конструктор для загрузки

    Map(const Map& other);
    Map& operator=(const Map& other);

    ~Map();

    int getWidth() const;
    int getHeight() const;

    const MapCell& getCell(const Utils::Point& position) const;
    MapCell& getCell(const Utils::Point& position);

    bool isValidPosition(const Utils::Point& position) const;
    
    void generateTerrain();

    void serialize(std::ostream& os) const;

private:
    void deserialize(std::istream& is);

    int mapWidth;
    int mapHeight;
    std::vector<std::vector<std::unique_ptr<MapCell>>> cells;

    void deepCopy(const Map& other);
    void setupInitialMap();
};

} 

#endif