#ifndef CELL_H
#define CELL_H

#include "entity.hpp"

class Cell {
public:
    enum CellType {
        GROUND,
        WALL,    
        SLOWING 
    };

private:
    CellType type;
    Entity* entity_on_cell;

public:
    Cell() : type(GROUND), entity_on_cell(nullptr) {}

    CellType getType() const {
        return type;
    }

    void setType(CellType new_type) {
        type = new_type;
    }

    Entity* getEntity() const {
        return entity_on_cell;
    }

    void setEntity(Entity* entity) {
        entity_on_cell = entity;
    }

    bool isPassable() const {
        return type != WALL && entity_on_cell == nullptr;
    }
};

#endif