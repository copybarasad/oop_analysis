#pragma once  

class Cell
{
private:
    char symbol;
    bool isWall; 
    bool isOccupied;
    int x; 
    int y;
    
public:
    Cell(char sym = '.', bool wall = false, bool occupied = false, int posX = 0, int posY = 0);
    
    char GetSymbol() const;
    bool IsWall() const;
    bool IsOccupied() const;
    void SetSymbol(char sym);
    void SetWall(bool wall);
    void SetOccupied(bool occupied);
    void Display() const;
    int GetX() const; 
    int GetY() const;
    void SetPosition(int posX, int posY);
};