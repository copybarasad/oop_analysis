#ifndef GAME_GAME_FIELD_H_
#define GAME_GAME_FIELD_H_
#include <vector>
#include <stdexcept>
#include "cell.h"
#include "player.h"
#include "enemy.h"
class GameField {
public:
    GameField(int rows, int cols);
    ~GameField() = default;
    GameField(const GameField& other);
    GameField& operator=(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(GameField&& other) noexcept;
    bool is_valid_position(int row, int col) const;
    bool is_empty(int row, int col) const;
    void place_player(const Player& player);
    void clear_cell(int row, int col);
    void move_player(int new_row, int new_col, Player& player);
    void move_enemy(int new_row, int new_col, Enemy& enemy, Player& player);
    void print_field(const Player& player, const std::vector<Enemy>& enemies) const;
    int rows() const { return rows_; }
    int cols() const { return cols_; }
    void update_cell(int row, int col, Cell::EntityType type);
private:
    int rows_;
    int cols_;
    std::vector<std::vector<Cell>> grid_;
};
#endif