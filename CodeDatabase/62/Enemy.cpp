#include "Enemy.h"
#include <iostream>

void Enemy::makeTurn(
    std::vector<std::pair<int, int>> path, 
    std::pair<int, int> player_pos,
    std::function<void(int x, int y)> makeMove,
    std::function<void(int damage, int x, int y)> applyDamage
) {
    size_t n = path.size();
    std::cout << "Making turn with path length " << n << std::endl;
    if(n > 1) {
        std::cout << "Moving to " << path.front().first << ", " << path.front().second << std::endl;
        makeMove(path.front().first, path.front().second);
    } else if(n == 1) {
        if(state_ == State::Idle) {
            state_ = State::Attacking;
            std::cout << "Attacking player at " << player_pos.first << ", " << player_pos.second << std::endl;
            applyDamage(getDamage(), player_pos.first, player_pos.second);
        }
    }
}