#ifndef OOP_FIELD_H
#define OOP_FIELD_H

#include <vector>
#include "cell.h"
#include "position.h"
#include "game_state.h"



class GameField {
    std::vector<std::vector<Cell>> grid;
    int width;
    int height;

    void initialize_grid();
    void generate_special_cells();

public:
    GameField(int width, int height);

    // правило пяти: конструкторы копирования и перемещения,
    // соответствующие им операторы присваивания и деструктор (здесь по умолчанию)
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    ~GameField() = default;


    bool position_is_valid(const Position& position) const;   // только через поле (проверка на границы)
    bool position_is_passable(const Position& position, GameState game_state) const;   // через состояние и поле, тк проверка на сущностей
    Position find_nearest_available_position(const Position& original_pos, const GameState& game_state) const;
    Cell get_cell_at(const Position& position) const;


    // обработка ловушек, ловушка не сущность, а тип клетки (она больше связаны с полем)
    void set_or_remove_trap(const Position& position, int trap_damage);   // trap_damage == 0 => удалить ловушку
    void set_cell_type(const Position& position, CellType cell_type);

    int get_width() const;
    int get_height() const;
};


#endif //OOP_FIELD_H