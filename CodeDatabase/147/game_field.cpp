#include "game_field.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

GameField::GameField(int rows, int columns, unsigned int seed): _seed(seed) {
    if (rows < 10) rows = 10;
    if (columns < 10) columns = 10;
    if (rows > 25) rows = 25;
    if (columns > 25) columns = 25;
    initializeGrid(rows, columns);
    if (_seed == 0)
        _seed = static_cast<unsigned>(std::time(nullptr));

    std::srand(_seed); 
    generateClosedCells();
}

GameField::GameField(int size, unsigned int seed): _seed(seed) {
    if (size < 10) size = 10;
    if (size > 25) size = 25;
    initializeGrid(size, size);
    if (_seed == 0)
        _seed = static_cast<unsigned>(std::time(nullptr));

    std::srand(_seed); 
    generateClosedCells();
}

void GameField::initializeGrid(int rows, int columns) {
    if (rows < 1 || columns < 1) {
        throw std::invalid_argument("Field size must be positive");
    }
    grid.clear();
    grid.resize(rows, std::vector<Cell>(columns, Cell()));
}

void GameField::generateClosedCells(double percentage) {
    int totalCells = getRowCount() * getColumnCount();
    int closedCount = static_cast<int>(totalCells * percentage);

    for (int i = 0; i < closedCount; ++i) {
        int row = std::rand() % getRowCount();
        int col = std::rand() % getColumnCount();

        int row_freeze = (std::rand() + 5) % getRowCount();
        int col_freeze = (std::rand() + 5) % getColumnCount();

        grid[row][col].setType(CellType::CLOSED);
        grid[row_freeze][col_freeze].setType(CellType::FREEZE);
    }
}

int GameField::getRowCount() const {
    return static_cast<int>(grid.size());
}

int GameField::getColumnCount() const {
    return grid.empty() ? 0 : static_cast<int>(grid[0].size());
}

const Cell& GameField::getCell(const Position& position) const {
    if (!position.isValid(getRowCount(), getColumnCount())) {
        throw std::out_of_range("Invalid position");
    }
    return grid[position.getRow()][position.getColumn()];
}

Cell& GameField::getCell(const Position& position) {
    if (!position.isValid(getRowCount(), getColumnCount())) {
        throw std::out_of_range("Invalid position");
    }
    return grid[position.getRow()][position.getColumn()];
}

void GameField::moveUnit(Unit* unit, Direction direction) {
    if (!unit || !unit->isAlive()) return;
    if (unit->get_type() == CellType::PLAYER && unit->isFrozen()) {
        unit->freeze();
        std::cout << "Player is frozen and cannot move.\n" << std::endl;
        return;
    }

    Position oldPos = unit->getPosition();
    Position newPos = oldPos.getNeighbor(direction);

    if (!newPos.isValid(getRowCount(), getColumnCount())) return;

    Cell& source = getCell(oldPos);
    Cell& destination = getCell(newPos);

    // Если на destination есть occupant — атакуем (player <-> enemy)
    if (destination.hasOccupant()) {
        Unit* defender = destination.getOccupant();
        Unit* attacker = source.getOccupant();

        if (defender && attacker) {
            bool enemy_vs_player =
                (defender->get_type() == CellType::ENEMY && attacker->get_type() == CellType::PLAYER) ||
                (defender->get_type() == CellType::PLAYER && attacker->get_type() == CellType::ENEMY);

            if (enemy_vs_player) {
                defender->takeDamage(attacker->get_damage());
            }
        }
        return;
    }

    // Если пустая и проходимая — двигаем
    if (destination.isPassable()) {
        if (unit->get_type() == CellType::PLAYER && destination.getType() == CellType::FREEZE) {
            unit->freeze();
        }
        source.clearOccupant();
        destination.setOccupant(unit);
        unit->setPosition(newPos);
        
    }
}

void GameField::removeUnit(Unit* unit) {
    if (!unit) return;
    Position pos = unit->getPosition();
    if (pos.isValid(getRowCount(), getColumnCount())) {
        Cell& c = getCell(pos);
        if (c.getOccupant() == unit) {
            c.clearOccupant();
        }
    }
}

void GameField::display() const {
    for (int row = 0; row < getRowCount(); ++row) {
        std::cout << row << " ";
        if (row < 10) std::cout << " ";
        for (int col = 0; col < getColumnCount(); ++col) {
            CellType type = grid[row][col].getType();
            switch (type) {
                case CellType::EMPTY: std::cout << ". "; break;
                case CellType::PLAYER: std::cout << "P "; break;
                case CellType::ENEMY: std::cout << "E "; break;
                case CellType::CLOSED: std::cout << "# "; break;
                case CellType::FREEZE: std::cout << "* "; break;
                case CellType::TOWER: std::cout << "T "; break;
                case CellType::E_TOWER: std::cout << "Y "; break;
                default: std::cout << "? "; break;
            }
        }
        std::cout << '\n';
    }
}

// Правило пяти: копирование/перемещение (реализованы просто копированием контейнеров)
GameField::GameField(const GameField& other)
    : grid(other.grid)
{
    // Зануляем occupant в каждой клетке
    for (auto& row : grid) {
        for (auto& cell : row) {
            if (cell.getOccupant() != nullptr){
                cell.clearOccupant();
            }
        }
    }
}
GameField::GameField(GameField&& other) noexcept
    : grid(std::move(other.grid))
{
    // Чистим occupant
    for (auto& row : grid) {
        for (auto& cell : row) {
            if (cell.getOccupant() != nullptr){
                cell.clearOccupant();
            }
        }
    }
}
GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        grid = other.grid;

        for (auto& row : grid) {
            for (auto& cell : row) {
                if (cell.getOccupant() != nullptr){
                    cell.clearOccupant();
                }
            }
        }
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept
{
    if (this == &other) return *this;

    grid = std::move(other.grid);

    // Чистим occupant
    for (auto& row : grid) {
        for (auto& cell : row) {
            if (cell.getOccupant() != nullptr){
                cell.clearOccupant();
            }
        }
    }
    return *this;
}

