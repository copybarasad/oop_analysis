#include "GameField.h"
#include <random>
#include <stdexcept>

GameField::GameField(int fieldWidth, int fieldHeight) : width(fieldWidth), height(fieldHeight) {
    if (fieldWidth < 10 || fieldWidth > 25 || fieldHeight < 10 || fieldHeight > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }
    grid.resize(height, std::vector<GameCell>(width));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int y=0;y<height;++y){
        for(int x=0;x<width;++x){
            double r = dis(gen);
            if (r < 0.08) grid[y][x] = GameCell(CellType::OBSTACLE);
            else if (r < 0.15) grid[y][x] = GameCell(CellType::SLOWING);
            else grid[y][x] = GameCell(CellType::EMPTY);
        }
    }
}

int GameField::getWidth() const { return width; }
int GameField::getHeight() const { return height; }

bool GameField::isValidPosition(const Position &position) const {
    return position.x>=0 && position.x<width && position.y>=0 && position.y<height;
}

const GameCell &GameField::getCell(const Position &position) const {
    if (!isValidPosition(position)) throw std::out_of_range("Position out of bounds");
    return grid[position.y][position.x];
}

bool GameField::isPositionPassable(const Position &position) const {
    if (!isValidPosition(position)) return false;
    return grid[position.y][position.x].isPassable();
}

void GameField::setCellOccupied(const Position &pos, bool occ) {
    if (!isValidPosition(pos)) return;
    grid[pos.y][pos.x].setOccupied(occ);
}

void GameField::setCellType(const Position& position, CellType type) {
    if (!isValidPosition(position)) return;
    grid[position.y][position.x] = GameCell(type);
}

void GameField::initializeFromData(int fieldWidth, int fieldHeight, 
                                  const std::vector<CellType>& cellTypes) {
    if (fieldWidth <= 0 || fieldHeight <= 0) {
        throw std::invalid_argument("Invalid field dimensions: " + 
            std::to_string(fieldWidth) + "x" + std::to_string(fieldHeight));
    }
    
    if (cellTypes.empty()) {
        throw std::invalid_argument("Cell types vector is empty");
    }
    
    size_t expectedCells = static_cast<size_t>(fieldWidth * fieldHeight);
    if (cellTypes.size() != expectedCells) {
        throw std::invalid_argument("Cell count mismatch. Got " + 
            std::to_string(cellTypes.size()) + ", expected " + std::to_string(expectedCells));
    }
    
    width = fieldWidth;
    height = fieldHeight;
    
    grid.clear();
    grid.resize(height, std::vector<GameCell>(width));
    int index = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (index >= static_cast<int>(cellTypes.size())) {
                throw std::out_of_range("Index out of range while loading field");
            }
            
            grid[y][x] = GameCell(cellTypes[index]);
            index++;
        }
    }
}