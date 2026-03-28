#include "GameObjects/Entities/Player.hpp"

Player::Player(PlayerDTO p) {
    this->damagePoints = p.dmg;
    this->disabled = p.disabled;
    this->healthPoints = p.hp;
    this->maxHP = p.maxHP;
    this->position = p.coord;
    this->rangeForm = p.rangeForm;
}

PlayerDTO Player::toDTO() {
    PlayerDTO dto;
    dto.coord = this->position;
    dto.disabled = this->disabled;
    dto.dmg = this->damagePoints;
    dto.hp = this->healthPoints;
    dto.maxHP = this->maxHP;
    dto.rangeForm = this->rangeForm;
    return dto;
}


bool Player::isRangeForm() {
    return rangeForm;
}

void Player::changeRangeForm() {
    if (rangeForm) {
        rangeForm = false;
        damagePoints = damagePoints * 2;
    } else {
        rangeForm = true;
        damagePoints = damagePoints / 2;
    }
}

void Player::setRangeForm(bool rf) {
    rangeForm = rf;
}

// void Player::loadFromDTO(PlayerDTO p) {
//     this->position = p.coord;
//     this->disabled = p.disabled;
//     this->damagePoints = p.dmg;
//     this->healthPoints = p.hp;
//     this->maxHP = p.maxHP;
//     this->rangeForm = p.rangeForm;
// }

