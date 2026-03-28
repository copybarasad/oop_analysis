#ifndef CELL_H
#define CELL_H

enum class CellType {
    EMPTY,
    WALL,
    SLOW_ZONE,
    TRAP
};

class Entity;

class Cell {
public:
    Cell(CellType type = CellType::EMPTY,
         Entity* entity = nullptr) 
         : type_(type),
            entity_(entity),
            trap_damage_(0) {}
    
    CellType get_type() const { return type_; }
    Entity* get_entity() const { return entity_; }
    int get_trap_damage() const { return trap_damage_; }
    
    bool is_occupied() const { return entity_ != nullptr; }
    bool is_passable() const { return type_ != CellType::WALL && !is_occupied(); }
    bool is_empty() const { return type_ == CellType::EMPTY; }
    void clear_entity() { entity_ = nullptr; }
    
    void set_type(CellType type) { type_ = type; }
    void set_entity(Entity* entity) { entity_ = entity; }
    void set_trap() { type_ = CellType::TRAP; }
    void set_trap_damage(int damage) { trap_damage_ = damage; }
    
private:
    CellType type_;
    Entity* entity_;
    int trap_damage_;
};

#endif