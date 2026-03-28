#include "Field.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Trap.h"
#include "Ally.h"
#include "Spawner.h"
#include <utility>

Field::Field(int rows, int cols) : rows_(rows), cols_(cols), grid_(rows * cols) {}

Field::Field(const Field& other)
        : rows_(other.rows_), cols_(other.cols_), grid_()
{
    grid_.reserve(rows_ * cols_);
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            const Cell& oc = other.grid_[r * cols_ + c];
            Cell nc(oc.type());
            if (oc.hasEntity()) {
                auto ent = oc.getEntity();
                if (ent) {
                    nc.setEntity(ent->clone());
                }
            }
            grid_.push_back(std::move(nc));
        }
    }
}

Field::Field(Field&& other) noexcept
        : rows_(other.rows_), cols_(other.cols_), grid_(std::move(other.grid_))
{
    other.rows_ = 0;
    other.cols_ = 0;
    other.grid_.clear();
}

Field& Field::operator=(const Field& other) {
    if (this == &other) return *this;
    Field tmp(other);
    std::swap(rows_, tmp.rows_);
    std::swap(cols_, tmp.cols_);
    std::swap(grid_, tmp.grid_);
    return *this;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this == &other) return *this;
    rows_ = other.rows_;
    cols_ = other.cols_;
    grid_ = std::move(other.grid_);
    other.rows_ = 0;
    other.cols_ = 0;
    other.grid_.clear();
    return *this;
}


bool Field::inBounds(Coord c) const {
    return c.row >= 0 && c.row < rows_ && c.col >= 0 && c.col < cols_;
}

Cell& Field::atIndex(int r, int c) { return grid_[r * cols_ + c]; }
const Cell& Field::atIndex(int r, int c) const { return grid_[r * cols_ + c]; }

Cell& Field::cellAt(Coord c) { return atIndex(c.row, c.col); }
const Cell& Field::cellAt(Coord c) const { return atIndex(c.row, c.col); }

void Field::setCellType(Coord c, CellType t) {
    if (!inBounds(c)) return;
    cellAt(c) = Cell(t);
    cellAt(c).clearEntity();
}

void Field::placeEntity(std::shared_ptr<Entity> e, Coord c) {
    if (!inBounds(c)) return;
    if (!cellAt(c).isPassable()) return;
    cellAt(c).setEntity(std::move(e));
}

void Field::removeEntity(Coord c) {
    if (!inBounds(c)) return;
    cellAt(c).clearEntity();
}

std::optional<Coord> Field::findPlayerCoord() const {
    for (int r = 0; r < rows_; ++r) for (int c = 0; c < cols_; ++c) {
            const auto &cell = grid_[r * cols_ + c];
            if (cell.hasEntity()) {
                if (dynamic_cast<Player*>(cell.getEntity().get())) return Coord{r,c};
            }
        }
    return std::nullopt;
}

std::optional<Coord> Field::findEntityCoord(std::shared_ptr<Entity> e) const {
    if (!e) return std::nullopt;
    for (int r = 0; r < rows_; ++r) for (int c = 0; c < cols_; ++c) {
            const auto &cell = grid_[r * cols_ + c];
            if (cell.getEntity() == e) return Coord{r,c};
        }
    return std::nullopt;
}

bool Field::spawnEnemyNear(Coord pos) {
    for (int dr = -1; dr <= 1; ++dr) for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            Coord c{pos.row + dr, pos.col + dc};
            if (!inBounds(c)) continue;
            if (!cellAt(c).hasEntity() && cellAt(c).isPassable()) {
                placeEntity(std::make_shared<Enemy>(6,2), c);
                return true;
            }
        }
    return false;
}


