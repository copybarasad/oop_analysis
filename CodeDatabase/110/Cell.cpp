#include "Cell.hpp"
#include "../../Entities/Player/Player.hpp"
#include "../../Entities/Enemies/Enemy.hpp"
#include "../../Entities/Inanimate/Building/Building.hpp"
#include "../../Entities/Inanimate/Trap/Trap.hpp"
#include "../../Entities/Inanimate/Tower/Tower.hpp"
#include "../../Entities/Ally/Ally.hpp"

Cell::Cell (size_t xValue, size_t yValue, CellType typeValue) :
    x(xValue), y(yValue), entity(nullptr), type(typeValue) {}

Cell::Cell (const Cell& other) : x(other.x), y(other.y), entity(nullptr), type(other.type) {}

Cell::Cell (Cell&& other) : x(other.x), y(other.y), entity(other.entity), type(other.type) {
    other.entity = nullptr;
}

Cell& Cell::operator= (const Cell& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        type = other.type;
    }
    return *this;
}

Cell& Cell::operator= (Cell&& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        entity = other.entity;
        type = other.type;
        other.entity = nullptr;
    }
    return *this;
}

void Cell::setEntity (Entity& placingEntity) {
    entity = &placingEntity;
}

void Cell::setType (CellType typeValue) { type = typeValue; }

size_t Cell::getX () const { return x; }

size_t Cell::getY () const { return y; }

Entity& Cell::getEntity () const { return *entity; }

CellType Cell::getType () const { return type; }

bool Cell::isEmpty () const { return entity == nullptr; }

bool Cell::isPlayer () const {
    return dynamic_cast<const Player*>(entity) != nullptr
    || dynamic_cast<const Trap*>(entity) != nullptr
    || dynamic_cast<const Ally*>(entity) != nullptr;
}

bool Cell::isEnemy () const {
    return dynamic_cast<const Enemy*>(entity) != nullptr
    || dynamic_cast<const Building*>(entity) != nullptr
    || dynamic_cast<const Tower*>(entity) != nullptr;
}

bool Cell::isBlocked () const { return type == CellType::BLOCKED; }

void Cell::clear () {
    entity = nullptr;
}