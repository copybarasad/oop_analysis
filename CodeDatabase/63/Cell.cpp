#include "Cell.h"

Cell::Cell() : passability(CellType::CLEAR), occupied(false), points(0) {}

Cell::Cell(CellType type) : passability(type), occupied(false), points(0) {}

Cell::Cell(json& data) : occupied(false) {
    if (!(data.is_array() && data.size() == 2))
        throw std::runtime_error("Одна из клеток игрового поля сохранена некорректно");

    if (!data[0].is_number_integer())
        throw std::runtime_error("Первое значение (тип проходимости) у клетки не является целочисленным значением");
    passability = data[0].get<CellType>();
    switch (passability) {
        case CellType::CLEAR:
        case CellType::SLOW:
        case CellType::WALL:
            break;
        default:
            throw std::runtime_error("Тип проходимости у клетки принимает некорректное значение");
    }

    if (!data[1].is_number_unsigned())
        throw std::runtime_error("Второе значение (количество очков) у клетки не является беззнаковым числом");
    points = data[1].get<unsigned int>();
}

json Cell::to_json() const { return {passability, points}; }

void Cell::setPassability(CellType new_type) { passability = new_type; }

void Cell::entityDied(const unsigned int& remainedPoints) {
    occupied = false;
    points = remainedPoints;
}

void Cell::entityLeaved() {
    occupied = false;
}

unsigned int Cell::entityPlaced() {
    occupied = true;
    unsigned int gainedPoints = points;
    points = 0;
    return gainedPoints;
}

CellType Cell::getPassability() const { return passability; }

int Cell::getPoints() const { return points; }

int Cell::collectPoints() {
    int p = points;
    points = 0;
    return p;
}

bool Cell::isOccupied() const { return occupied; }

bool Cell::isPassable() const { return passability != CellType::WALL; }