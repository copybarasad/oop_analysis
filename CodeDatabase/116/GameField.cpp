#include "GameField.h"

GameField::GameField(const GameField& other):width(other.width),height(other.height),grid(other.grid){
}

GameField::GameField(int w, int h):width(w), height(h){
    if (w < 10 || h < 10 || w > 25 || h > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }
    grid.resize(height);
    for (int i = 0; i < height; ++i) {
        grid[i].resize(width);
        for (int j = 0; j < width; j++) {
            grid[i][j] = Cell(Cell::CellType::EMPTY);
        }
    }
}

GameField::~GameField() = default;

GameField& GameField::operator=(const GameField& other){
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

GameField::GameField(GameField&& other):width(other.width),height(other.height),grid(std::move(other.grid)){
    other.width = 0;
    other.height = 0;
}


GameField& GameField::operator=(GameField&& other){
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

const Cell& GameField::getCell(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("Coordinates out of bounds");
    }
    return grid[y][x];
}

Cell& GameField::getCell(int x, int y){
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("Coordinates out of bounds");
    }
    return grid[y][x];
}

bool GameField::isValidMove(int x, int y) const{
    if (x < 0 || x >= width|| y < 0 || y >= height) {
        return false;
    }
    return getCell(x, y).getType() != Cell::CellType::WALL;
}

bool GameField::isStructure(int x, int y) const{
    return (getCell(x, y).getType() == Cell::CellType::BUILDING) || (getCell(x, y).getType() == Cell::CellType::FORTRESS);
}

int GameField::getWidth() const {return width;}

int GameField::getHeight() const {return height;}

void GameField::setCell(int x, int y, Cell cell){
    grid[y][x] = cell;  // Правильный порядок индексов
}