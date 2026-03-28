#include "GameField.h"
#include "SaveLoad.h"
#include <stdexcept>
#include <random>
#include <chrono>
#include <set>

GameField::GameField(int fielWidth, int fieldHeight, const std::vector<Position>& reservedPositions) : width(fielWidth), height(fieldHeight) {
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }

    std::set<Position> reservedSet(reservedPositions.begin(), reservedPositions.end());

    initializeField(&reservedSet);
}

void GameField::initializeField(const std::set<Position>* reserved) {
    cells.resize(height, std::vector<Cell>(width, Cell(0, 0)));

    std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Position pos(x, y);

            if (reserved && reserved->count(pos)) {
                cells[y][x] = Cell(x, y, true, CellType::EMPTY);
                continue;
            }

            double obstacleChance = 0.2;     // 20%

            if ((x <= 2 && y <= 2) ||
                (x >= width - 3 && y <= 2) ||
                (x <= 2 && height - 3) ||
                (x >= width - 3 && y <= height - 3)) {
                obstacleChance = 0.05;      // 5% в углах
                }

            bool should_be_passable = (dis(gen) > obstacleChance);

            if (!should_be_passable) {
                cells[y][x] = Cell(x, y, false, CellType::WALL);
            } else {
                cells[y][x] = Cell(x, y, true, CellType::EMPTY);
            }
        }
    }

    // Свободные углы
    if (!(reserved && reserved->count(Position(0, 0)))) {
        cells[0][0].setIsPassable(true);
    }
    if (!(reserved && reserved->count(Position(0, 1)))) {
        cells[0][1].setIsPassable(true);
    }
    if (!(reserved && reserved->count(Position(1, 0)))) {
        cells[1][0].setIsPassable(true);
    }
    if (!(reserved && reserved->count(Position(0, width - 1)))) {
        cells[0][width - 1].setIsPassable(true);
    }
    if (!(reserved && reserved->count(Position(height - 1, 0)))) {
        cells[height - 1][0].setIsPassable(true);
    }
    if (!(reserved && reserved->count(Position(height - 1, width - 1)))) {
        cells[height - 1][width - 1].setIsPassable(true);
    }
}

GameField::GameField(const GameField& other) : width(other.width), height(other.height) {
    cells = other.cells;
}

GameField::GameField(GameField&& other) noexcept : width(other.width), height(other.height), cells(std::move(other.cells)) {
    other.width = 0;
    other.height = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        cells = other.cells;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        cells = std::move(other.cells);

        other.width = 0;
        other.height = 0;
    }
    return *this;
}

bool GameField::isValidPosition(const Position& position) const {
    return position.x >= 0 && position.x < width &&
           position.y >= 0 && position.y < height;
}

bool GameField::isPositionValid(const Position& position) const {
    return isValidPosition(position);
}

bool GameField::isCellPassable(const Position& position) const {
    if (!isValidPosition(position)) {
        return false;
    }

    const Cell& cell = cells[position.y][position.x];
    return cell.getType() != CellType::WALL && cell.getType() != CellType::TOWER;
}

bool GameField::canMoveTo(const Position& newPosition) const {
    if (!isValidPosition(newPosition) || !isCellPassable(newPosition)) {
        return false;
    }
    return true;
}

int GameField::getWidth() const {
    return width;
}
int GameField::getHeight() const {
    return height;
}

const Cell& GameField::getCell(const Position& position) const {
    if (!isValidPosition(position)) {
        throw std::out_of_range("Position is out of field bounds");
    }
    return cells[position.y][position.x];
}

Cell& GameField::getCell(const Position& position) {
    if (!isValidPosition(position)) {
        throw std::out_of_range("Position is out of field bounds");
    }
    return cells[position.y][position.x];
}

// === JSON ===
const std::vector<std::vector<Cell>>& GameField::getCells() const {
    return cells;
}

void GameField::loadState(const nlohmann::json& j) {
    int new_width = j.at("width").get<int>();
    int new_height = j.at("height").get<int>();

    if (new_width != width || new_height != height) {
        width = new_width;
        height = new_height;
        cells.clear();
        cells.resize(height, std::vector<Cell>(width, Cell(0, 0)));
    }

    const auto& cells_json = j.at("cells");
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Position pos(x, y);
            Cell temp_cell(x, y);
            cells_json[y][x].get_to(temp_cell);
            getCell(pos) = std::move(temp_cell);
        }
    }
}