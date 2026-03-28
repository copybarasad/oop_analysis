#pragma once

#include <iostream>
#include <string>

enum class CellType 
{
    empty,
    wall,
    swamp,
    nydus,
    exit_cell
};

class Cell 
{
protected:
    CellType type;
    std::string defTile;
    std::string Tile;
public:
    Cell(CellType type = CellType::empty);
    void CellSetTile(std::string Body);
    void CellResetTile();
    CellType CellGetType() const;
    std::string CellGetTile() const;
    bool CellIsntWall() const;
    void CellPlot();
};