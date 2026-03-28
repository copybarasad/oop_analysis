#include "Cell.h"

Cell::Cell(Type type) : m_type(type) {}

Cell::Type Cell::getType() const {
    return m_type;
}

void Cell::setType(Type type) {
    m_type = type;
}

char Cell::getChar() const {
    switch (m_type) {
        case EMPTY: return ' ';
        case WALL: return '#';
        case OBSTACLE: return '@';
        default: return '?';
    }
}