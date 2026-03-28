#include "enemy.hpp"
#include <cmath>

Enemy::Enemy(int hp, int damage, Point pos)
    : Actor(hp, damage, pos) {}

Enemy::~Enemy() = default;

bool Enemy::StepOrHit(const Board& board, const Point& player_pos, int& out_damage_to_player) {
    out_damage_to_player = 0;
    int dx = 0, dy = 0;
    if (player_pos.x < pos_.x) dx = -1; else if (player_pos.x > pos_.x) dx = 1;
    if (player_pos.y < pos_.y) dy = -1; else if (player_pos.y > pos_.y) dy = 1;

    auto try_move = [&](int mx, int my) -> bool {
        int nx = pos_.x + mx;
        int ny = pos_.y + my;
        if (nx == player_pos.x && ny == player_pos.y) {
            out_damage_to_player = base_damage_;
            return false;
        }
        if (board.InBounds(nx, ny) && board.IsPassable(nx, ny)) {
            pos_.x = nx; pos_.y = ny; 
            return true;
        }
        return false;
    };

    bool moved = false;
    if (std::abs(player_pos.x - pos_.x) >= std::abs(player_pos.y - pos_.y)) {
        moved = try_move(dx, 0);
        if (!moved) moved = try_move(0, dy);
    } else {
        moved = try_move(0, dy);
        if (!moved) moved = try_move(dx, 0);
    }
    return moved;
}