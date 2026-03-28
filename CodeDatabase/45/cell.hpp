#ifndef CELL_HPP
#define CELL_HPP

#include "entity.hpp"
#include <string>

class Map;

enum class CellState { 
    EMPTY, 
    PLAYER, 
    MONSTER,
    WALL,
    TRAP,
    TOWER
};

class Cell {
private:
    CellState state;
    Entity* entity;

public:
    Cell();
    ~Cell() = default;

    void transferEntityTo(Cell& other);

    CellState getState() const;
    void setEntity(Entity* newEntity, CellState newState);
    void removeEntity();
    void applyDamage(int damageAmount);
    int getEntityDamage() const;
    bool getEntityCanMove() const;
    void setEntityCanMove(bool can);
    void setWall(bool wall);
    void setTrap(bool trap);
    int getEntityHp() const;
    void entityMakeStep(Map* map, int x, int y);

    std::string getCellStatus() const;
    
};

#endif