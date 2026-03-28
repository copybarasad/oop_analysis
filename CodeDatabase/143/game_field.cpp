#include "game_field.h"
#include <stdexcept>
#include <fstream>

GameField::GameField(int width, int height)
    : fieldWidth(width), fieldHeight(height) {

    if (width < GameConstants::MIN_FIELD_SIZE ||
        width > GameConstants::MAX_FIELD_SIZE ||
        height < GameConstants::MIN_FIELD_SIZE ||
        height > GameConstants::MAX_FIELD_SIZE) {
        throw std::invalid_argument("Invalid field dimensions");
    }

    initializeGrid();
}

GameField::GameField(const GameField& other)
    : fieldWidth(other.fieldWidth),
    fieldHeight(other.fieldHeight) {
    copyFrom(other);
}

GameField::GameField(GameField&& other) noexcept {
    moveFrom(std::move(other));
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        fieldWidth = other.fieldWidth;
        fieldHeight = other.fieldHeight;
        copyFrom(other);
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        moveFrom(std::move(other));
    }
    return *this;
}

int GameField::getWidth() const {
    return fieldWidth;
}

int GameField::getHeight() const {
    return fieldHeight;
}

CellType GameField::getCellType(const Position& position) const {
    if (!isValidPosition(position)) {
        throw std::out_of_range("Position is out of field bounds");
    }
    return grid[position.getY()][position.getX()].getType();
}

bool GameField::isValidPosition(const Position& position) const {
    return position.getX() >= 0 && position.getX() < fieldWidth &&
        position.getY() >= 0 && position.getY() < fieldHeight;
}

bool GameField::isPositionEmpty(const Position& position) const {
    return isValidPosition(position) &&
        grid[position.getY()][position.getX()].isEmpty();
}

void GameField::setCellType(const Position& position, CellType type) {
    if (!isValidPosition(position)) {
        throw std::out_of_range("Position is out of field bounds");
    }
    grid[position.getY()][position.getX()].setType(type);
}

void GameField::clearCell(const Position& position) {
    setCellType(position, CellType::EMPTY);
}

void GameField::initializeGrid() {
    grid.resize(fieldHeight);
    for (int y = 0; y < fieldHeight; y++) {
        grid[y].resize(fieldWidth);
        for (int x = 0; x < fieldWidth; x++) {
            grid[y][x] = Cell();
        }
    }
}

void GameField::copyFrom(const GameField& other) {
    grid = other.grid;
}

void GameField::moveFrom(GameField&& other) noexcept {
    fieldWidth = other.fieldWidth;
    fieldHeight = other.fieldHeight;
    grid = std::move(other.grid);

    other.fieldWidth = 0;
    other.fieldHeight = 0;
}

void GameField::save(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&fieldWidth), sizeof(fieldWidth));
    out.write(reinterpret_cast<const char*>(&fieldHeight), sizeof(fieldHeight));
    for (const auto& row : grid)
        for (const auto& cell : row)
            cell.save(out);
}

void GameField::load(std::istream& in) {
    in.read(reinterpret_cast<char*>(&fieldWidth), sizeof(fieldWidth));
    in.read(reinterpret_cast<char*>(&fieldHeight), sizeof(fieldHeight));
    grid.resize(fieldHeight);
    for (auto& row : grid) {
        row.resize(fieldWidth);
        for (auto& cell : row)
            cell.load(in);
    }
}