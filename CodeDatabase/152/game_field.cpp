#include "game_field.h"
#include <iostream>

GameField::GameField(int rows, int cols)
    : rows_(rows), cols_(cols), grid_(rows, std::vector<Cell>(cols)) {
    if (rows < 1 || cols < 1) {
        throw std::invalid_argument("Field size must be positive.");
    }
}

GameField::GameField(const GameField& other)
    : rows_(other.rows_), cols_(other.cols_), grid_(other.grid_) {
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        rows_ = other.rows_;
        cols_ = other.cols_;
        grid_ = other.grid_;
    }
    return *this;
}

GameField::GameField(GameField&& other) noexcept
    : rows_(other.rows_), cols_(other.cols_), grid_(std::move(other.grid_)) {
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        rows_ = other.rows_;
        cols_ = other.cols_;
        grid_ = std::move(other.grid_);
    }
    return *this;
}

bool GameField::is_valid_position(int row, int col) const {
    return row >= 0 && row < rows_ && col >= 0 && col < cols_;
}

bool GameField::is_empty(int row, int col) const {
    return is_valid_position(row, col) &&
        grid_[row][col].type() == Cell::EntityType::kEmpty;
}

void GameField::update_cell(int row, int col, Cell::EntityType type) {
    if (is_valid_position(row, col)) {
        grid_[row][col].set_type(type);
    }
}

void GameField::place_player(const Player& player) {
    update_cell(player.row(), player.col(), Cell::EntityType::kPlayer);
}

void GameField::clear_cell(int row, int col) {
    update_cell(row, col, Cell::EntityType::kEmpty);
}

void GameField::move_player(int new_row, int new_col, Player& player) {
    if (!is_valid_position(new_row, new_col)) return;
    if (!is_empty(new_row, new_col)) return;

    clear_cell(player.row(), player.col());
    player.set_position(new_row, new_col);
    place_player(player);
}

void GameField::move_enemy(int new_row, int new_col, Enemy& enemy, Player& player) {
    if (!is_valid_position(new_row, new_col)) return;

    if (new_row == player.row() && new_col == player.col()) {
        player.take_damage(enemy.damage());
        return;
    }

    if (!is_empty(new_row, new_col)) return;

    clear_cell(enemy.row(), enemy.col());
    enemy.set_position(new_row, new_col);
    update_cell(new_row, new_col, Cell::EntityType::kEnemy);
}

void GameField::print_field(const Player& player, const std::vector<Enemy>& enemies) const {
    std::vector<std::vector<char>> temp(rows_, std::vector<char>(cols_, '.'));

    temp[player.row()][player.col()] = 'P';

    for (const auto& e : enemies) {
        if (e.is_alive()) {
            if (is_valid_position(e.row(), e.col()))
                temp[e.row()][e.col()] = 'E';
        }
    }

    std::cout << "   ";
    for (int c = 0; c < cols_; ++c) {
        std::cout << c << ' ';
    }
    std::cout << '\n';

    for (int r = 0; r < rows_; ++r) {
        std::cout << r << ": ";
        for (int c = 0; c < cols_; ++c) {
            std::cout << temp[r][c] << ' ';
        }
        std::cout << '\n';
    }
}

