#ifndef UNTITLED_FIELD_H
#define UNTITLED_FIELD_H

#include <vector>
#include <memory>
#include <optional>
#include <tuple>
#include "Cell.h"
#include "Coord.h"

class Entity;
class Player;

class Field {
public:
    Field(int rows = 14, int cols = 14);

    Field(const Field& other);

    Field(Field&& other) noexcept;

    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;

    int rows() const noexcept { return rows_; }
    int cols() const noexcept { return cols_; }

    bool inBounds(Coord c) const;

    Cell& cellAt(Coord c);
    const Cell& cellAt(Coord c) const;

    std::optional<Coord> findPlayerCoord() const;

    void setCellType(Coord c, CellType t);

    void placeEntity(std::shared_ptr<Entity> e, Coord c);
    void removeEntity(Coord c);

    bool spawnEnemyNear(Coord pos);

private:
    int rows_;
    int cols_;
    std::vector<Cell> grid_;
    Cell& atIndex(int r, int c);
    const Cell& atIndex(int r, int c) const;
    std::optional<Coord> findEntityCoord(std::shared_ptr<Entity> e) const;
};

#endif //UNTITLED_FIELD_H

