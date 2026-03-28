#include "dracula.h"
#include "field.h"
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Dracula::Dracula(const Position& position)
    : Character("Дракула", 100, position),
      fear_level_(0) {}

void Dracula::takeDamage(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    
    health_ -= amount;

    if (health_ < 0) {
        health_ = 0;
    }
    if (health_ > 200) {
        health_ = 200;
    }
}

void Dracula::applyCellEffect(CellType cell_type) {
    if (cell_type == CellType::VAMPIRE_LAIR) {
        health_ += 5;
        if (health_ > 200) health_ = 200;
    }
    else if (cell_type == CellType::CHAPEL) {
        takeDamage(10);  
    }
}

void Dracula::calculateFearLevel(int hunter_research) {
    if (hunter_research >= 3) {
        fear_level_ = 1;  // ПАНИКА (убегает)
    } else {
        fear_level_ = 0;  // АГРЕССИЯ (преследует)
    }
}

std::pair<int, int> Dracula::calculateMoveDirection(const Position& hunter_position) const {
    int dx = position_.deltaX(hunter_position);
    int dy = position_.deltaY(hunter_position);
    int distance = position_.manhattanDistance(hunter_position);
    
    int move_x = 0;
    int move_y = 0;
    
    if (std::abs(dx) > std::abs(dy)) {
        move_x = normalizeComponent(dx);
    } else if (std::abs(dy) > 0) {
        move_y = normalizeComponent(dy);
    }
    
    switch (fear_level_) {
        case 0:  // Агрессия - преследуем охотника
            return {move_x, move_y};
        case 1:  // ПАНИКА - убегаем
            return {-move_x, -move_y};  
        default:
            return {move_x, move_y};
    }
}

bool Dracula::makeMove(const Position& hunter_pos, int hunter_research, Field& field) {
    calculateFearLevel(hunter_research);
    if (position_.manhattanDistance(hunter_pos) == 1 && fear_level_ == 0) {
        return false; // Остаемся на месте - будем атаковать
    }
    auto [dx, dy] = calculateMoveDirection(hunter_pos);
    
    if (dx != 0 || dy != 0) {
        if (moveWithField(dx, dy, field)) {
            return true;
        }
    }
    
    // Если основное движение не удалось, пробуем альтернативные направления
    std::vector<std::pair<int, int>> directions = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    
    for (auto [alt_dx, alt_dy] : directions) {
        if (moveWithField(alt_dx, alt_dy, field)) {
            return true;
        }
    }
    
    return false;  
}

int Dracula::getFearLevel() const {
    return fear_level_;
}
