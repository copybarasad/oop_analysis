#ifndef CELL_HPP
#define CELL_HPP

#include "entities/entity.hpp"

enum class CellType {
    Default,
    Slow,
    Impassible,
};

class Cell {
    CellType type = CellType::Default;
    Entity *entity {nullptr};
    int trap_damage {0};


public:
    Cell(CellType type = CellType::Default);
    void set_trap(int damage);
    bool has_trap() const;
    int get_trap_damage() const;
    void set_trap_damage(int damage);
    CellType get_type() const;
    void set_type(CellType type);  
    void set_entity(Entity *entity);
    bool has_entity() const;
    EntityType get_entity_type();
    Entity &get_entity();
};

#endif
