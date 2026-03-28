#pragma once
#include <vector>
#include <optional>
#include "Cell.h"
#include "Position.h"

class Field {
public:
    Field(int rows, int cols);
    Field(const Field& other);
    Field& operator=(const Field& other) = default;
    Field(Field&& other) noexcept = default;
    Field& operator=(Field&& other) noexcept = default;

    int rows() const;
    int cols() const;

    const Cell& cellAt(const Position& p) const;
    Cell& cellAt(const Position& p);

    bool inBounds(const Position& p) const;
    std::optional<Position> randomFreePosition() const;

    void save(std::ostream& os) const;
    void load(std::istream& is);

private:
    void validateSize(int rows, int cols) const;

    int rows_;
    int cols_;
    std::vector<std::vector<Cell>> grid_;
};
