#ifndef CELL_H
#define CELL_H

#include <string>

class Cell {
    bool is_occupied;
    bool has_player;
    bool has_enemy;
    bool is_blocked;
    bool has_tower;
    bool has_trap;
    
public:
    Cell();
    
    void clear();
    void setPlayer();
    void setEnemy();
    void setTower();
    void setTrap();
    void removeTrap();
    void setBlocked(bool blocked);
    
    bool getIsOccupied() const;
    bool getHasPlayer() const;
    bool getHasEnemy() const;
    bool getHasTower() const;
    bool getHasTrap() const;
    bool getIsBlocked() const;
    
    std::string serialize() const;
    void deserialize(char data);
};

#endif