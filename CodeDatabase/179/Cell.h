#ifndef CELL_H
#define CELL_H

#include "CellType.h"
#include <memory>

class GameObject;

class Cell {
private:
    CellType type;
    std::shared_ptr<GameObject> object;

public:
    Cell(CellType type = CellType::NORMAL);
    
    CellType getType() const;
    void setType(CellType newType);
    
    bool hasObject() const;
    std::shared_ptr<GameObject> getObject() const;
    void setObject(std::shared_ptr<GameObject> newObject);
    void removeObject();
    
    bool isPassable() const;
    bool isPassableForMovement() const;  // Убедитесь что этот метод есть
};

#endif
