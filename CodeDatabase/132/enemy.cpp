#include "enemy.hpp"

std::vector<Weapon> ENEMY_WEAPONS = { Arms() };
const int ENEMY_SPEED = 1;
const int ENEMY_HEALTH = 40;

Enemy::Enemy():
    MovableEntity(ENEMY_HEALTH),
    Armed(ENEMY_WEAPONS)
{
    screen_symbol = 'E';
}

Enemy::Enemy(MobImage enemy_image):
    MovableEntity(enemy_image.health_points, enemy_image.can_move),
    Armed(ENEMY_WEAPONS)
{
    screen_symbol = 'E';
}

MobImage Enemy::get_data_image() {
    return MobImage(
        health_points,
        can_move,
        get_effects_img()
    );
}

// TODO: bot
// TODO: nearest -> remove
Move Enemy::choose_move(const std::vector<Position> &step_moves, const std::vector<Position> &attack_moves, const Position &cur_pos, const Position &nearest) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<Position> avail_moves;
    for (auto &pos : attack_moves) {
        if (Position::manhattan_distance(pos, cur_pos) > 1) continue;
        avail_moves.push_back(pos);
    }
    for (auto &pos : step_moves) {
        if (Position::manhattan_distance(pos, cur_pos) > 1) continue;
        avail_moves.push_back(pos);
    }

    std::vector<Position> best_moves;
    int min_dist = INF;

    for (auto &pos : avail_moves) {
        int cur_dist = Position::manhattan_distance(pos, nearest);
        if (cur_dist < min_dist) {
            min_dist = cur_dist;
        }
    }

    for (auto &pos : avail_moves) {
        int cur_dist = Position::manhattan_distance(pos, nearest);
        if (cur_dist == min_dist) {
            best_moves.push_back(pos);
        }
    }

    std::uniform_int_distribution<> dis(0, best_moves.size() - 1);

    Position pos = best_moves[dis(gen)];

    if (Position::manhattan_distance(pos, nearest) == 0) {
        return { cur_pos, pos, MoveType::ATTACK };
    }

    return { cur_pos, pos, MoveType::MOVE };
}

GameObject* Enemy::clone() const {
    return new Enemy(*this);
}