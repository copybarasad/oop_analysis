#include "../include/entity/player.h"

Player::Player() : Entity(100), name_("Hero"), score_(0), weapon_(), inventory_(5), hand_(), has_trap_(false), 
                enhancement_(), max_health_bonus_(0), weapon_damage_bonus_(0), spell_power_bonus_(0), max_inventory_bonus_(0) {}

Player::Player(std::string name, int max_health) : Entity(max_health), name_(name), score_(0), weapon_(), inventory_(5), hand_(), enhancement_(), max_health_bonus_(0), weapon_damage_bonus_(0), spell_power_bonus_(0), max_inventory_bonus_(0) {}

const std::string Player::name() const { return this->name_; }

int Player::damage() const { return this->weapon_.damage(); }

int Player::score() const { return this->score_; }

void Player::add_score(int points) {
    if (points < 0) { return; }
    this->score_ += points;
}

bool Player::spend_score(int amount) {
    if (amount < 0 || this->score_ < amount) {
        return false;
    }
    this->score_ -= amount;
    return true;
}

Position Player::NextPosition(Direction dir) const {
    int row = 0;
    int col = 0;
    
    switch (dir) {
        case Direction::Up:    row = -1; break;
        case Direction::Down:  row = 1; break;
        case Direction::Left:  col = -1; break;
        case Direction::Right: col = 1; break;
    }
    
    return Position(row, col);
}

const Weapon &Player::weapon() const {
    return this->weapon_;
}

void Player::change_weapon() {
    if (this->weapon_.type() == WeaponType::Sword) {
        this->weapon_.set_type(WeaponType::Bow);
    } else {
        this->weapon_.set_type(WeaponType::Sword);
    }
}

const Inventory& Player::inventory() const {
    return this->inventory_;
}

Inventory& Player::inventory() {
    return this->inventory_;
}

const Hand& Player::hand() const {
    return this->hand_;
}

Hand& Player::hand() {
    return this->hand_;
}

void Player::heal(int amount) {
    if (amount < 0) return;
    
    int new_health = health() + amount;
    if (new_health > max_health()) {
        new_health = max_health();
    }
    
    set_health(new_health);
}

bool Player::has_trap() const {
    return this->has_trap_;
}

void Player::set_trap(bool has_trap) {
    this->has_trap_ = has_trap;
}