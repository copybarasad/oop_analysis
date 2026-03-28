#ifndef CELL_H
#define CELL_H

enum class CellType{
    PASSABLE,
    BLOCKED
};

class Cell
{
private:
    CellType currentcell;
public:
    Cell();
    CellType GetCellType() const;
    void SwitchCellType(CellType new_type);
    bool IsPassable() const;
    bool IsBlocked() const;
};

#endif