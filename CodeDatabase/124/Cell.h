#ifndef CELL_H
#define CELL_H

class Cell {
private:
    bool isOccupied;
    bool hasPlayer;
    bool hasEnemy;
    bool isPassable;

public:
    Cell();
    
    bool getIsOccupied() const;
    bool getHasPlayer() const;
    bool getHasEnemy() const;
    bool getIsPassable() const;
    void setHasPlayer(bool value);
    void setHasEnemy(bool value);
    void setIsPassable(bool value);
    void clear();
};

#endif