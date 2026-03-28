#include "cell.hpp"
#include "enums.hpp"

Cell::Cell()
    : type_(CellType::kEmpty),
      entity_id_(-1) {}

CellType Cell::getType() const noexcept {
    return type_;
}

int Cell::getEntityId() const noexcept {
    return entity_id_;
}

void Cell::set(CellType t, int id) noexcept {
    type_ = t;
    entity_id_ = id;
}

void Cell::clear() noexcept {
    type_ = CellType::kEmpty;
    entity_id_ = -1;
}
