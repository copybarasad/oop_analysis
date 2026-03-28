#include <vector>
#include <iostream>
#include <random>
#include "cell.h"
#include "game_field.h"
#include "entity.h"

GameField::GameField(int sizeX, int sizeY) : sizeX(sizeX), sizeY(sizeY) {
    field.resize(sizeY);
    for (auto &elem : field){
        elem.reserve(sizeX);
        for (int i = 0; i < sizeX; ++i) {
            elem.emplace_back(PASSABLE);
        }
    }

    int total_cells = sizeX * sizeY;
    int impassable_count = total_cells * 5 / 100;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(0, sizeX - 1);
    std::uniform_int_distribution<> distY(0, sizeY - 1);
    int placed = 0;
    while (placed < impassable_count) {
        int x = distX(gen);
        int y = distY(gen);
        if ((field[y][x].getType() != IMPASSABLE) && (x!=0 || y!=0) && (x!=sizeX/2 || y!=sizeY/2)) {
            field[y][x] = Cell(IMPASSABLE);
            placed++;
        }
    }
}


GameField::GameField(const GameField& other)
    : sizeX(other.sizeX), sizeY(other.sizeY), field(other.sizeY)
{
    for (int y = 0; y < sizeY; ++y) {
        field[y].reserve(sizeX);
        for (int x = 0; x < sizeX; ++x) {
            field[y].emplace_back(other.field[y][x]);
        }
    }
}


GameField::GameField(GameField&& other) noexcept
    : sizeX(other.sizeX), sizeY(other.sizeY), field(std::move(other.field))
{
    other.sizeX = 0;
    other.sizeY = 0;
}


GameField& GameField::operator=(const GameField& other)
{
    if (this != &other) {
        sizeX = other.sizeX;
        sizeY = other.sizeY;
        field.clear();
        field.resize(sizeY);
        for (int y = 0; y < sizeY; ++y) {
            field[y].reserve(sizeX);
            for (int x = 0; x < sizeX; ++x) {
                field[y].emplace_back(other.field[y][x]);
            }
        }
    }
    return *this;
}


GameField& GameField::operator=(GameField&& other) noexcept
{
    if (this != &other) {
        sizeX = other.sizeX;
        sizeY = other.sizeY;
        field = std::move(other.field);
        other.sizeX = 0;
        other.sizeY = 0;
    }
    return *this;
}

void GameField::reinitialize(int newWidth, int newHeight){
    *this = GameField(newWidth, newHeight);
    sizeX = newWidth;
    sizeY = newHeight;
}

int GameField::getSizeX() const { return sizeX; }

int GameField::getSizeY() const { return sizeY; }

Cell& GameField::getCell(int y, int x) const {
    return const_cast<Cell&>(field[y][x]);
}

void GameField::setCellType(int y, int x, CellType celltype){
    field[y][x] = Cell(celltype);
}


void GameField::showField() const {
    for (int y = 0; y < sizeY; ++y) {
        for (int x = 0; x < sizeX; ++x) {
            const Cell& cell = field[y][x];
            if (cell.hasEntity()) {
                std::cout << cell.getEntity()->getSym();
            } else {
                std::cout << cell.getSymbol();
            }
            std::cout << " "; // Comment if you want narrow field
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

