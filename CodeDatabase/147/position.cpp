#include "position.h"
#include "direction.h"

Position::Position(int row, int column) : row(row), column(column) {
    if (row < 0) this->row = 0;
    if (column < 0) this->column = 0;
}

int Position::getRow() const {
    return row;
}

int Position::getColumn() const {
    return column;
}

bool Position::isValid(int maxRows, int maxColumns) const {
    return row >= 0 && column >= 0 && row < maxRows && column < maxColumns;
}

Position Position::getNeighbor(Direction direction) const {
    switch (direction) {
        case Direction::UP: return Position(row - 1, column);
        case Direction::DOWN: return Position(row + 1, column);
        case Direction::LEFT: return Position(row, column - 1);
        case Direction::RIGHT: return Position(row, column + 1);
        default: return *this;
    }
}

bool Position::operator==(const Position& other) const {
    return row == other.row && column == other.column;
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}