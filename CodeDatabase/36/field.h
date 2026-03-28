#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include "cell.h"

class Field {
public:
    Field(int width = 10, int height = 10);
    ~Field() = default;
    Field(const Field& other);
    Field& operator=(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(Field&& other) noexcept;
    int GetWidth() const;
    int GetHeight() const;
    bool IsInBounds(int r, int c) const;
    Cell& CellAt(int r, int c);
    const Cell& CellAt(int r, int c) const;
    void Print() const;
    void Initialize(int level);

private:
    void Initialize();
    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;
};

#endif