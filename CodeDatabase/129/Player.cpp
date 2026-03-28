#include "Player.h"

Player::Player() : Entity() {
    lifes = PLAYER::P_LIFES;
    close_combat = true;
    damage = PLAYER::CLOSE_COMBAT_DAMAGE;
}

void Player::set_combat(bool c_c){
    close_combat = c_c;
    if(close_combat){
        damage = PLAYER::CLOSE_COMBAT_DAMAGE;
    } else {
        damage = PLAYER::RANGE_COMBAT_DAMAGE;
    }
}

bool Player::is_close_combat(){
    return close_combat;
}


Saving::PlayerData Player::serialize() const {
    return {lifes, damage, coords.x, coords.y, close_combat};
}

void Player::deserialize(const Saving::PlayerData& data) {
    lifes = data.lifes;
    damage = data.damage;
    coords.x = data.x;
    coords.y = data.y;
    close_combat = data.close_combat;
}

Player::~Player() {}

