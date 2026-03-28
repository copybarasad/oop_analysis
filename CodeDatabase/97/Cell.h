#ifndef UNTITLED_CELL_H
#define UNTITLED_CELL_H



#include <memory>

class Entity;

enum class CellType {
    Empty, Obstacle, Slow
};

class Cell {
public:
    Cell(CellType type = CellType::Empty);

    CellType type() const noexcept;

    bool isPassable() const noexcept;

    bool isSlow() const noexcept;

    void setEntity(std::shared_ptr<Entity> e);

    void clearEntity();

    std::shared_ptr<Entity> getEntity() const;

    bool hasEntity() const;

private:
    CellType type_;
    std::shared_ptr<Entity> entity_;
};


#endif //UNTITLED_CELL_H
