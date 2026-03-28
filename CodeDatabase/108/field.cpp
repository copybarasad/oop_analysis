#include "field.h"
#include <random>
#include <algorithm>
#include <stdexcept>
#include "game_state.h"


GameField::GameField(int width, int height) {
    if (width < 10) {
        this->width = 10;
    } else if (width > 25) {
        this->width = 25;
    } else {
        this->width = width;
    }

    if (height < 10) {
        this->height = 10;
    } else if (height > 25) {
        this->height = 25;
    } else {
        this->height = height;
    }

    initialize_grid();
    generate_special_cells();
}



void GameField::initialize_grid() {
    grid.resize(height);
    for (auto& row : grid) {
        row.resize(width, Cell(CellType::EMPTY));
    }
}


void GameField::generate_special_cells() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);      // 10% шанс препятствия, 10% шанс замедления
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int random_value = dis(gen);
            if (random_value == 0) {
                grid[y][x] = Cell(CellType::OBSTACLE);     // 10% препятствий
            } else if (random_value == 1) {
                grid[y][x] = Cell(CellType::SLOW);         // 10% замедляющих
            }
            // остальные 80% - EMPTY (по умолчанию)
        }
    }

    grid[0][0] = Cell(CellType::EMPTY);      // для начальной позиции игрока (в основном будущем цикле игры) --- done
    grid[1][0] = Cell(CellType::EMPTY);
    grid[0][1] = Cell(CellType::EMPTY);      // чтобы игрок не был заблокирован

}



// конструктор копирования
GameField::GameField(const GameField& other) = default;

// конструктор перемещения из other в this
GameField::GameField(GameField&& other) noexcept :
    grid(std::move(other.grid)), width(other.width),
    height(other.height) {
    other.width = 0;
    other.height = 0;
    other.grid.clear();

}

// оператор присваивания с копированием
GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

// оператор присваивания с перемещением
GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);

        other.width = 0;
        other.height = 0;
        other.grid.clear();
    }
    return *this;
}




bool GameField::position_is_valid(const Position& position) const {
    return position.get_x() >= 0 &&
           position.get_x() < width &&
           position.get_y() >= 0 &&
           position.get_y() < height;
}


// нет препятствия и других сущностей
bool GameField::position_is_passable(const Position& position, GameState game_state) const {
    if (!position_is_valid(position)) {
        return false;
    }

    // выносим логику в лямбда выражение во избежание дублирования кода (NEW)
    auto check_entity = [&](const Entity& entity) {
        return entity.get_position() == position;
    };

    for (const Enemy& enemy : game_state.get_enemies())
        if (check_entity(enemy)) return false;  // нашли занятую - сразу выходим (optimization)
    for (const EnemySpawner& spawner : game_state.get_spawners())
        if (check_entity(spawner)) return false; // нашли занятую - сразу выходим
    for (const EnemyTower& tower : game_state.get_towers())
        if (check_entity(tower)) return false;   // нашли занятую - сразу выходим

    return grid[position.get_y()][position.get_x()].is_passable();
}


// For collision solving
Position GameField::find_nearest_available_position(const Position& original_pos, const GameState& game_state) const{

    // ищем по спирали от оригинальной позиции
    for (int radius = 1; radius <= std::max(width, height); ++radius) {
        for (int dx = -radius; dx <= radius; ++dx) {
            for (int dy = -radius; dy <= radius; ++dy) {

                // Проверяем только внешний слой спирали
                if (std::abs(dx) == radius || std::abs(dy) == radius) {
                    Position candidate(original_pos.get_x() + dx, original_pos.get_y() + dy);

                    if (position_is_valid(candidate) && position_is_passable(candidate, game_state)) {
                        return candidate;
                    }
                }
            }
        }
    }

    // Если не нашли подходящую позицию (вдруг все занято) => исключение
    throw std::runtime_error(
        "No available position found for entity at (" +
        std::to_string(original_pos.get_x()) + "," +
        std::to_string(original_pos.get_y()) + "). " +
        "The field is too crowded!"
    );
}


int GameField::get_width() const {
    return width;
}

int GameField::get_height() const {
    return height;
}


Cell GameField::get_cell_at(const Position& position) const {
    if (!position_is_valid(position)) {
        throw std::out_of_range("Position is outside field borders");
    }
    return grid[position.get_y()][position.get_x()];
}


void GameField::set_or_remove_trap(const Position& position, int trap_damage) {
    if (trap_damage == 0) {
        grid[position.get_y()][position.get_x()].set_type(CellType::EMPTY);  // удаляем ловушку
        grid[position.get_y()][position.get_x()].set_trap_damage(0);
    } else {
        grid[position.get_y()][position.get_x()].set_type(CellType::TRAP);   // там нет ловушки, ставим туда
        grid[position.get_y()][position.get_x()].set_trap_damage(trap_damage);
    }
}

void GameField::set_cell_type(const Position &position, CellType cell_type) {
    grid[position.get_y()][position.get_x()].set_type(cell_type);
}

