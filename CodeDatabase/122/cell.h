#ifndef CELL_H
#define CELL_H

#include "entity.h"

class Cell
{
public:
    Cell();
    ~Cell() = default;

    enum OccupationStatus
    {
        FREE,
        TAKEN,
    };

    CellType getType() const {return  m_type; }
    OccupationStatus getState() const {return m_state;}
    void setType(CellType type) { m_type = type; }

    void free();
    void occupy();
    void block();
    void original();

protected:
    OccupationStatus m_state;
    CellType m_type;
};

#endif // CELL_H
