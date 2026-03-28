#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <memory>
#include <string>

#include "player.hpp"
#include "cell.hpp"
#include "monster.hpp"



class Player;

class Map {
private:
    std::vector<std::vector<Cell>> grid;
    int width;
    int height;
    int playerX;
    int playerY;

public:
    Map(int mapWidth, int mapHeight);
    
    Map(const Map& other); // Конструктор копирования
    Map(Map&& other) noexcept; // Конструктор перемещения

    ~Map();

    Map& operator=(const Map& other); // Оператор присваивания с копированием
    Map& operator=(Map&& other) noexcept; // Оператор присваивания с перемещением
    
    void print() const;
    bool addPlayer(int x, int y, Player* playerPtr);
    bool addEntity(int x, int y, std::unique_ptr<Entity> entity, CellState state);
    CellState getCellState(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    bool moveEntity(int fromX, int fromY, int toX, int toY);
    int getMonsterCount() const;
    int getPlayerX() const;
    int getPlayerY() const;

    void setTrapAt(int x, int y);
    void setWallAt(int x, int y, bool wall);

    void applyDamageAt(int x, int y, int damage);

    void everyoneCanWalk();

    void makeStep(int x, int y);

    void clearMap();

    std::string getCellStatusAt(int x, int y) const;


};

#endif