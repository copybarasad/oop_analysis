#pragma once
#include <iostream>
#include <memory>
#include "Cell.h"
#include <cstdlib>
#include <algorithm> 
#include <vector>
#include <utility>
#include "GameExceptions.h"


class Player;
class Enemy;


class Field{
private:

    int width, height;
    std::vector<std::vector<Cell>> grid;

public:
    Field(int widthField = 10, int heightField = 10);
    
    Field(const Field& other);
    Field& operator=(const Field& other);

    Field(Field&& other)noexcept;
    Field& operator=(Field&& other)noexcept;

    ~Field();


    bool canMoveTo(int x, int y) const;

    CellType getCellType(int x, int y) const;

    void printCellInfo(int x, int y) const;

    int getWidth() const {return width;};

    int getHeight() const {return height;};

    GameEnt* getCellEntity(int x, int y) const;

    bool cellIsEmpty(int x, int y) const;

    // Не константные методы

    void setCellType(CellType type, int x, int y);

    int setCellEntity(GameEnt* entity, int x, int y);

    int clearCellEnt(int x, int y);

    int clearCellType(int x, int y);

    void clearAllEntities();

    //save and load
    void save(std::ostream& out) const;
    void load(std::istream& in);
};

