#include "cell.h"

Cell::Cell() : m_state(FREE), m_type(CellType::ORIGINAL) {}

void Cell::free()
{
    m_state = FREE;
}

void Cell::occupy()
{
    m_state = TAKEN;
}

void Cell::block()
{
    m_type = CellType::BLOCKED;
}

void Cell::original()
{
    m_type = CellType::ORIGINAL;
    m_state = FREE;
}
