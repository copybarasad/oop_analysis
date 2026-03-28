#ifndef CELL_H
#define CELL_H

#include "../Constants.h"

enum class CellType {Empty = 1, Wall, Slowing};

const CellType DEFAULT_CELL_TYPE = CellType::Empty;

class Cell{
private:
    CellType cellType;
    bool busysness;
public:
    Cell();
    Cell(CellType cellType);
    bool isPass() const;
    bool isFree() const;
    CellType getCellType() const;
    bool getBusysness() const;
    void setCellType(CellType cellType);
    void setBusysness(bool busysness);
};

#endif