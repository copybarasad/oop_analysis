#include "enemy.h"
#include "game_field.h"
#include "cell.h"

std::vector<int> Enemy::calculate_new_move(const Player& player, GameField& field) {
    int dx = player.getPosX() - this->getPosX();
    int dy = player.getPosY() - this->getPosY();
    std::vector<int> new_move(2);
    int hor_move = std::abs(dx) == 0 ? 0 : dx / std::abs(dx);
    int vert_move = std::abs(dy) == 0 ? 0 : dy / std::abs(dy);

    if (std::abs(dx) <= std::abs(dy)) {
        if (field.getCell(vert_move + this->getPosY(), this->getPosX()).getType() == IMPASSABLE){
            new_move = {hor_move, 0};
        }
        else new_move = {0, vert_move};
    } else {
        if (field.getCell(this->getPosY(), this->getPosX() + hor_move).getType() == IMPASSABLE){
            new_move = {0, vert_move};
        }
        else new_move = {hor_move, 0};
    }
    return new_move;
}