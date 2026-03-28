#ifndef GAME_CELL_H_
#define GAME_CELL_H_

class Cell {
public:
    enum class EntityType {
        kEmpty,
        kPlayer,
        kEnemy
    };

    Cell() = default;
    explicit Cell(EntityType type) : type_(type) {}

    EntityType type() const { return type_; }
    void set_type(EntityType type) { type_ = type; }

private:
    EntityType type_ = EntityType::kEmpty;
};

#endif // GAME_CELL_H_
