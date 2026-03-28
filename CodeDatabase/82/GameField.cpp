#include "GameField.h"
#include "WallGenerator.h"
#include <stdexcept>  

GameField::GameField(int fieldWidth, int fieldHeight) : width(fieldWidth), height(fieldHeight) {
    validateDimensions();
    initializeGrid();
}

void GameField::validateDimensions() const {
    if (width < 10) throw std::invalid_argument("Ширина должна быть больше 10");
    if (height < 10) throw std::invalid_argument("Длинна должна быть больше 10");
    if (width > 25) throw std::invalid_argument("Ширина должна быть больше 25");
    if (height > 25) throw std::invalid_argument("Длинна должна быть больше 25");
}

void GameField::initializeGrid() {
    grid.resize(height, std::vector<Cell>(width, Cell(CellType::EMPTY)));
    WallGenerator::generateRandomWalls(*this, 0.2);
}

const Cell& GameField::getCell(int x, int y) const {
    if (!isPositionValid(Position(x, y))) {
        throw std::out_of_range("Неправильные координаты для стены");
    }
    return grid[y][x];
}

bool GameField::isPositionValid(const Position& position) const {
    return position.x >= 0 && position.x < width && position.y >= 0 && position.y < height;
}

bool GameField::isCellPassable(const Position& position) const {
    return isPositionValid(position) && grid[position.y][position.x].isPassable();
}

void GameField::setCellType(const Position& position, CellType cellType) {
    if (isPositionValid(position)) {
        grid[position.y][position.x].setType(cellType);
    }
}
