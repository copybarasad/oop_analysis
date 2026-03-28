#pragma once
#include <vector>
#include <stdexcept>
#include <random>
#include <iostream>
#include <cmath>
#include "GameObjects/Field.hpp"
#include "LevelManager/LevelManager.hpp"
#include "GameObjects/Entities/Player.hpp"
#include "GameObjects/Entities/Enemy.hpp"
#include "GameObjects/Entities/Tower.hpp"
#include "GameObjects/Map.hpp"


class Map {
public:
    Map(MapDTO map);
    void putMapSave(MapDTO map);

    Map(const Map& other);
    Map(Map&& other);

    Map& operator=(const Map& other);
    Map& operator=(Map&& other);

    Field& getField(Coordinates coord);
    Field& getField(int x, int y);

private:
    std::vector<std::vector<Field>> fields;
};
