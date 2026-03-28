#include "ally.hpp"
#include <cmath>
#include <iostream>
#include "constants.hpp"

Ally::Ally(const std::string& name, int health, int damage, int x, int y)
    : Hero(name, health, damage), x_(x), y_(y) {}

void Ally::SetPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

void Ally::Move(std::vector<Enemy>& enemies, Field& field, const Player& player) {
    if (!IsAlive() || enemies.empty()) return;

    Enemy* nearest = nullptr;
    int bestDist = MAX_DISTANCE;

    for (auto& e : enemies) {
        if (!e.IsAlive()) continue;
        int dist = std::abs(e.GetX() - x_) + std::abs(e.GetY() - y_);
        if (dist < bestDist) {
            bestDist = dist;
            nearest = &e;
        }
    }

    if (!nearest) return;

    int dx = 0, dy = 0;
    if (nearest->GetX() > x_) dx = STEP;
    else if (nearest->GetX() < x_) dx = STEP_BACK;
    else if (nearest->GetY() > y_) dy = STEP;
    else if (nearest->GetY() < y_) dy = STEP_BACK;

    int nx = x_ + dx;
    int ny = y_ + dy;

    if (nx >= FIELD_MIN && ny >= FIELD_MIN && nx < field.width() && ny < field.height()) {
        bool enemy_on_cell = false;
        for (auto& e : enemies) {
            if (e.IsAlive() && e.GetX() == nx && e.GetY() == ny) {
                enemy_on_cell = true;
                break;
            }
        }

        if (nx == player.GetX() && ny == player.GetY()) return;

        if (!enemy_on_cell && field.At(nx, ny).type() == CellType::kEmpty) {
            field.At(x_, y_).SetType(CellType::kEmpty);
            SetPosition(nx, ny);
            field.At(nx, ny).SetType(CellType::kAlly);
        }
    }
}