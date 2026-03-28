#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include <vector>
#include <memory>
#include "plot.hpp"
#include "../entities/character.hpp"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
#include "../auxil/loadexc.hpp"
#include "../auxil/file.hpp"
#include <ctime>
#include <algorithm>

using json = nlohmann::json;

class GameMap {
private:
    int width;
    int height;
    std::vector<std::vector<Plot>> grid;
    int trapDamage = 30;
    
public:
    GameMap(std::string name);
    GameMap(int width, int height);
    GameMap(const GameMap& other);
    GameMap(GameMap&& other) noexcept;
    
    GameMap& operator=(const GameMap& other);
    GameMap& operator=(GameMap&& other) noexcept;

    void saveMap(json& j);
    void loadMap(json& j);
    bool isValidPosition(int x, int y);
    bool isPositionEmpty(int x, int y);
    bool isPositionPassable(int x, int y);
    Plot& getPlot(int x, int y);
    
    void placeCharacter(Character* character, int x, int y);
    void removeCharacter(int x, int y);
    Character* getCharacterAt(int x, int y);
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void findSpawnPositionNearEntity(int X, int Y, int& emptyX, int& emptyY);
    void setTrap(int x, int y);

    int getTrapDamage() const { return trapDamage; }
    void setTrapDamage(int value) { trapDamage = value; }

    void display() const;
    
private:
    void initializeGrid();
};

#endif