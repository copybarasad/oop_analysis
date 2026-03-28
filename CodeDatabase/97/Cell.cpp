#include "Cell.h"
#include "Entity.h"

Cell::Cell(CellType type) : type_(type), entity_(nullptr) {}

CellType Cell::type() const noexcept { return type_; }
bool Cell::isPassable() const noexcept { return type_ != CellType::Obstacle; }
bool Cell::isSlow() const noexcept { return type_ == CellType::Slow; }

void Cell::setEntity(std::shared_ptr<Entity> e) { entity_ = std::move(e); }
void Cell::clearEntity() { entity_.reset(); }
std::shared_ptr<Entity> Cell::getEntity() const { return entity_; }
bool Cell::hasEntity() const { return (bool)entity_; }
