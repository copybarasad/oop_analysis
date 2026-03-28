#include "player.hpp"

Player::Player(int x, int y, int hp_, int dmg) 
    : Entity(x, y, Faction::Ally, hp_, dmg), stars(0) {}


int Player::getStars() const {
    return stars;
}

void Player::addStars(int val) {
    stars += val;
}

void Player::setStars(int val) {
    stars = val;
}