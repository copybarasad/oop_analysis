#pragma once
#include "enums.hpp"

class Cell {
public:
    Cell();

    CellType getType() const noexcept;
    int getEntityId() const noexcept;

    void set(CellType t, int id = -1) noexcept;
    void clear() noexcept;

private:
    CellType type_;
    int entity_id_;
};
