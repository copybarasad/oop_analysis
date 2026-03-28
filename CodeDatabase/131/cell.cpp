#include "cell.h"

namespace rpg {
    Cell::Cell() : type_(CellType::kNormal) {
    }

    Cell::Cell(CellType type) : type_(type) {
    }

    [[nodiscard]] bool Cell::IsPassable() const {
        return type_ != CellType::kImpassable;
    }

    [[nodiscard]] bool Cell::IsSlowing() const {
        return type_ == CellType::kSlowing;
    }

    [[nodiscard]] CellType Cell::GetType() const {
        return type_;
    }

    void Cell::SetType(CellType type) {
        type_ = type;
    }
}
