#include "player.h"
Player::Player(int health, int damage, int score)
    : health_(health), damage_(damage), score_(score) {
}
void Player::take_damage(int damage) {
    if (damage > 0) {
        health_ -= damage;
        if (health_ < 0) health_ = 0;
    }
}
void Player::add_score(int points) {
    if (points > 0) {
        score_ += points;
    }
}
bool Player::is_alive() const {
    return health_ > 0;
}
void Player::set_position(int row, int col) {
    row_ = row;
    col_ = col;
}