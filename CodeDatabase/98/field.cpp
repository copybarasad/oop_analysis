#include <iostream>
#include <cstdlib>
#include "field.h"
#include "cell.h"

Field::Field(int w, int h): width(w), height(h), grid(h, std::vector<Cell>(w)) {} //grid[h][w]
int Field::GetWidth() const { 
    return width; 
}
int Field::GetHeight() const { 
    return height; 
}
Cell& Field::GetCell(int x, int y){
    return grid[y][x];
}
const Cell& Field::GetCell(int x, int y) const { 
    return grid[y][x]; 
}
void Field::AddBlockCell(double chance){
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            if (static_cast<double>(rand()) / RAND_MAX < chance) //static_cast<double>(rand()) / RAND_MAX; преобразует в дабл rand(); это рандомное число; RAND_MAX контаснта библиолтеки 
                grid[y][x].SwitchCellType(CellType::BLOCKED);
}

// копирубщий и оператор присваивания с копированием;
Field::Field(const Field& other): width(other.width), height(other.height), grid(other.grid) {}
Field& Field::operator=(const Field& other) {
    if ( this == &other ) return *this;
    width = other.width;
    height = other.height;
    grid = other.grid;
    return *this;
}
// перемещзения и оператор присваивания с премещением; 
Field::Field(Field&& other) noexcept: width(other.width), height(other.height), grid(std::move(other.grid)) {}
Field& Field::operator=(Field&& other) noexcept{
    if ( this == &other ) return *this;
    width = other.width;
    height = other.height;
    grid = std::move(other.grid);
    return *this;
}