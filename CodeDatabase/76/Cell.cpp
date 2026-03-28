#include <iostream>
#include "Cell.h"

using namespace std;

Cell::Cell(char sym, bool wall, bool occupied, int posX, int posY)
    : symbol(sym), isWall(wall), isOccupied(occupied), x(posX), y(posY) 
{
}

int Cell::GetX() const {
    return x;
}

int Cell::GetY() const {
    return y;
}

void Cell::SetPosition(int posX, int posY) {
    x = posX;
    y = posY;
}

char Cell::GetSymbol() const { return symbol; }
bool Cell::IsWall() const { return isWall; }
bool Cell::IsOccupied() const 
{ 
    if (isWall && isOccupied) 
    {
        throw logic_error("Cell state inconsistency: wall cannot be occupied");
    }
    return isOccupied; 
}

void Cell::SetSymbol(char sym) { symbol = sym; }
void Cell::SetWall(bool wall) { isWall = wall; }
void Cell::SetOccupied(bool occupied) 
{
    if (isWall && occupied) 
    {
        throw invalid_argument("Wall cannot be occupied");
    }
    isOccupied = occupied;
}

void Cell::Display() const
{ 
    cout << symbol;
}