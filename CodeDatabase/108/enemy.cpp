#include "enemy.h"
#include <algorithm>
#include "field.h"
#include <iostream>
#include <utility>

Enemy::Enemy(std::string name, const Position& position, int health, int damage, int range) :
    Entity(std::move(name), position, health, damage), attack_range(range) {}


void Enemy::move_towards_player(const Position& player_position, GameState& game_state) {  // учитываем препятствия!!!
    if (!is_alive()) return;


    Position new_position = position;
    int delta_x = position.get_x() - player_position.get_x();
    int delta_y = position.get_y() - player_position.get_y();

    // двигаемся в том направлении, по какому расстоянию больше
    if (std::abs(delta_x) > std::abs(delta_y)) {
        new_position.set_x(position.get_x() + (delta_x > 0 ? -1 : 1));
    } else {
        new_position.set_y(position.get_y() + (delta_y > 0 ? -1 : 1));
    }


    // используем перегруженный оператор сравнения
    // если дошел до игрока, то не шагает, а (в основном цикле) будет наносить урон
    // так как будет истинным значение метода can_attack_player
    if (new_position == player_position) return;

    if (game_state.get_field().position_is_valid(new_position) && game_state.get_field().position_is_passable(new_position, game_state)) {
        position = new_position;
    } else {
        find_alternative_path(player_position, game_state);
    }
}



void Enemy::find_alternative_path(const Position& player_position, GameState& game_state) {
    std::vector<Position> possible_moves = {
        Position(position.get_x() + 1, position.get_y()),  // right
        Position(position.get_x() - 1, position.get_y()),  // left
        Position(position.get_x(), position.get_y() + 1),  // up
        Position(position.get_x(), position.get_y() - 1)   // down
    };

    Position best_move = position;
    int min_distance = 1000;

    for (const auto& move : possible_moves) {
        if (game_state.get_field().position_is_valid(move) &&
            game_state.get_field().position_is_passable(move, game_state) &&
            (!(move == player_position))) {

            int distance = std::abs(move.get_x() - player_position.get_x()) + std::abs(move.get_y() - player_position.get_y());

            if (distance <= min_distance) {
                min_distance = distance;
                best_move = move;
            }
        }
    }

    if ( !(best_move == position) ) {
        position = best_move;

    } else {
        std::cout << "Враг заблокирован! " << std::endl;         // маловероятно, но такое возможно
    }
}




bool Enemy::can_attack_player(const Position& player_position) const {   // может наносить урон только если его следующий ход - клетка с игроком (не как игрок - наискосок)
    int delta_x = std::abs(player_position.get_x() - position.get_x());
    int delta_y = std::abs(player_position.get_y() - position.get_y());
    return delta_x + delta_y == 1;
}

int Enemy::get_attack_range() const {
    return attack_range;
}
