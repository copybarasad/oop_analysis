#include "map_cell.h"

MapCell::MapCell(Type cell_type, bool use) : type(cell_type), use(use) {}

MapCell::Type MapCell::getType() const { return type; }

void MapCell::setType(Type new_type) { type = new_type; }

bool MapCell::isUsed() const {
    return use || entity != nullptr || type == Type::WALL;
}

void MapCell::setUsed(bool used) {
    use = used;
}

Entity* MapCell::getEntity() { 
    return entity; 
}

const Entity* MapCell::getEntity() const { 
    return entity; 
}

void MapCell::setEntity(Entity* ent) { 
    entity = ent; 
}


