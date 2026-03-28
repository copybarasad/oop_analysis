#include "Field.h"
#include <iostream>

Field::Field(int w, int h, const std::vector<std::pair<int, int>>& walls)
    : width(w), height(h)
{
    if (w < 10 || w > 25 || h < 10 || h > 25) {
        throw std::invalid_argument("Field dimensions must be positive");
    }    
    grid.resize(height, std::vector<Cell>(width));

    for(const auto& wall : walls){
        int x = wall.first;
        int y = wall.second;
        if(isValidPos(x, y)){
            grid[y][x].isWall = true;
        }
    }
}

void Field::copyField(const Field& other) {
    width = other.width;
    height = other.height;
    grid.resize(other.height, std::vector<Cell>(other.width));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = other.grid[y][x];
        }
    }
}

Field::Field(Field&& other) noexcept
    : width(other.width), height(other.height), grid(std::move(other.grid)) {
    other.width = 0;
    other.height = 0;
    other.grid.clear();
}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        copyField(other);
    }
    return *this;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);

        other.width = 0;
        other.height = 0;
        other.grid.clear();
    }
    return *this;
}

bool Field::isValidPos(int x, int y) const {
    return x>= 0 && x < width && y >= 0 && y < height;
}

bool Field::isPassable(int x, int y) const {
    return isValidPos(x, y) && !grid[y][x].isWall;
}

bool Field::isCellEmpty(int x, int y) const {
    return isValidPos(x, y) && grid[y][x].occupantID == -1;
}

bool Field::canMoveTo(int x, int y) const {
    return isPassable(x,y) && isCellEmpty(x,y);
}

void Field::placeCharacter(int ID, int x, int y, bool isPlayer, bool isEnemy){
    if (!isValidPos(x, y) || !isPassable(x, y)) {
        throw std::logic_error("Cannot place character: cell is wall or invalid");
    }
    if (!isCellEmpty(x, y)) {
        throw std::logic_error("Cannot place character: cell is occupied");
    }
    grid[y][x].occupantID = ID;
    grid[y][x].isPlayer = isPlayer;
    grid[y][x].isEnemy = isEnemy;

}

void Field::removeCharacter(int x, int y){
    if (!isValidPos(x, y)) return;
    grid[y][x].occupantID = -1;
    grid[y][x].isPlayer = false;
    grid[y][x].isEnemy = false;
}

void Field::moveCharacter(int oldX, int oldY, int newX, int newY, int ID, bool isPlayer, bool isEnemy){
    if (!isValidPos(oldX, oldY) || grid[oldY][oldX].occupantID != ID){
        throw std::logic_error("Character not found at old position");
    }
    if (!canMoveTo(newX, newY)){
        throw std::logic_error("Cannot move to target cell: wall or occupied");
    }

    removeCharacter(oldX, oldY);
    placeCharacter(ID, newX, newY, isPlayer, isEnemy);
}


const Field::Cell& Field::getCell(int x, int y) const {
    if (!isValidPos(x, y)) {
        throw std::out_of_range("Invalid cell position");
    }
    return grid[y][x];
}


void Field::save(std::ostream& out) const {

    out.write(reinterpret_cast<const char*>(&width), sizeof(width));
    out.write(reinterpret_cast<const char*>(&height), sizeof(height));

    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            const auto& cell = grid[y][x];
            out.write(reinterpret_cast<const char*> (&cell.isWall), sizeof(cell.isWall));
            out.write(reinterpret_cast<const char*> (&cell.occupantID), sizeof(cell.occupantID));
            out.write(reinterpret_cast<const char*> (&cell.isPlayer), sizeof(cell.isPlayer));
            out.write(reinterpret_cast<const char*> (&cell.isEnemy), sizeof(cell.isEnemy));
        }
    }
}


void Field::load(std::istream& in){
    int newWidth, newHeight;
    in.read(reinterpret_cast<char*>(&newWidth), sizeof(newWidth));
    in.read(reinterpret_cast<char*>(&newHeight), sizeof(newHeight));

    if(newWidth < 10 || newWidth > 25 || newHeight < 10 || newHeight > 25){
        throw std::runtime_error("Invalid field dimensions is save file.");
    }

    grid.resize(height, std::vector<Cell>(width));

    for (int y = 0; y < newHeight; ++y){
        for (int x = 0; x < newWidth; ++x){
            auto& cell = grid[y][x];
            in.read(reinterpret_cast<char*>(&cell.isWall), sizeof(cell.isWall));
            in.read(reinterpret_cast<char*>(&cell.occupantID), sizeof(cell.occupantID));
            in.read(reinterpret_cast<char*>(&cell.isPlayer), sizeof(cell.isPlayer));
            in.read(reinterpret_cast<char*>(&cell.isEnemy), sizeof(cell.isEnemy));
        }
    }
    width = newWidth;
    height = newHeight;
}