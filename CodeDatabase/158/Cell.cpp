#include "Cell.h"

Cell::Cell(CellType type):type(type) 
{
    switch(type) 
    {
        case CellType::empty: defTile = "◌ "; break;
        case CellType::wall: defTile = "⌧ "; break;
        case CellType::swamp: defTile = "… "; break;
        case CellType::nydus: defTile = "಄ "; break;
        case CellType::exit_cell: defTile = "⍞ "; break;
    }
    Tile = defTile;
}

void Cell::CellSetTile(std::string Body) { Tile = Body; }
void Cell::CellResetTile() { Tile = defTile; }
CellType Cell::CellGetType() const { return type; }
std::string Cell::CellGetTile() const { return Tile; }
bool Cell::CellIsntWall() const { return type != CellType::wall; }
void Cell::CellPlot() { std::cout << Tile; }